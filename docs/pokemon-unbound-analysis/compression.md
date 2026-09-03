# Compression

## Signatures candidates fournies

`compression-signatures.csv` : 308 777 lignes, colonnes `offset, gba, type, declared_output_size`.
Répartition par type :

| Type | Occurrences | Part |
|---|---|---|
| LZ77 | 132 047 | 42,8 % |
| Huffman | 107 393 | 34,8 % |
| RLE | 69 337 | 22,4 % |

Ces trois algorithmes sont **CONFIRMÉS (connaissance publique)** comme les trois méthodes de compression
standard supportées nativement par le BIOS GBA (`LZ77UnComp`, `HuffUnComp`, `RLUnComp`), utilisées de
façon quasi universelle par les jeux GBA pour les graphismes/tuiles. Leur présence en tant que
*signatures candidates* dans le pack est donc attendue — mais leur nombre brut ne dit rien sur leur
fiabilité individuelle (voir ci-dessous).

## Le signal est probablement bruité à un niveau significatif

Observation sur les données du pack elles-mêmes : la région 0 (0x0–0x3FFF), qui contient par convention
GBA l'en-tête et le début du code exécutable (donc très majoritairement du code, pas des données
compressées), affiche 359 signatures de compression candidates. Un vrai jeu ne compresse quasiment
jamais son code de démarrage. **PROBABLE, confiance moyenne-haute** : l'heuristique de détection de
signature (probablement un scan de motifs d'en-tête LZ77/Huffman/RLE sur des fenêtres glissantes) génère
un taux de faux positifs significatif sur du code ou des données non compressées, par coïncidence
d'octets.

Conséquence : `comp_sig_count` seul n'est pas fiable pour distinguer "compressé" de "non compressé". Il
n'est utilisé dans `rom-map.md` qu'en combinaison avec une entropie très haute (≥7.3, proche du maximum
théorique de 8.0 bits/octet) ET peu de chaînes de caractères détectées — la conjonction des trois réduit
le risque de faux positif, sans l'éliminer.

## Zones à examiner en priorité (si accès à la ROM était possible)

D'après `data/region-profile.csv`, les régions avec l'entropie la plus proche du maximum théorique (>7.8)
sont les candidats les plus solides pour des données réellement compressées :

| Région | Offset | Entropie | comp_sig_count | Chaînes |
|---|---|---|---|---|
| 1128 | 0x11A0000 | 7.72 | 13 | 970 |
| 1119 | 0x117C000 | 7.80 | 41 | 742 |
| 805 | 0xC94000 | 7.73 | 55 | 628 |

Note : ces trois régions ont AUSSI un nombre élevé de chaînes détectées, ce qui contredit en partie
l'hypothèse "données compressées pures" — elles ont été classées `table_texte_probable` dans `rom-map.md`
plutôt que `donnees_compressees_probable`, précisément à cause de ce chevauchement. HYPOTHÈSE non
tranchée : soit ces régions contiennent un mélange (texte suivi de graphismes compressés dans la même
fenêtre de 16 Ko), soit les motifs Huffman/RLE se déclenchent aussi par coïncidence sur du texte encodé
(un texte avec beaucoup de répétitions de caractères peut ressembler statistiquement à du RLE).

## Ce qui n'est PAS déterminable avec ce pack

- La taille réelle décompressée de chaque bloc au-delà du champ `declared_output_size` fourni (lui-même
  une heuristique, pas une valeur lue depuis un en-tête réel de compression vérifié).
- Le contenu des données une fois décompressées (graphisme, tuile, son) — nécessiterait la ROM et un
  décompresseur, hors périmètre de cette analyse.
- Toute distinction entre "compression standard BIOS" et un éventuel format de compression propriétaire
  ajouté par Unbound/CFRU au-delà du standard FireRed.
