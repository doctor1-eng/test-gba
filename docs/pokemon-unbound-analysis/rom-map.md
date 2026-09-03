# Cartographie théorique de la ROM

Générée par `tools/unbound_analysis/correlate_regions.py`, qui découpe la ROM en 2048 régions de 16 KiB
(alignées sur la granularité de `entropy-16k.csv`, la plus fine fournie) et corrèle les sept autres CSV
sur chaque région. Sortie complète : `data/region-profile.csv` (2048 lignes, consultable directement).

**Toute classification ci-dessous est une HYPOTHÈSE dérivée de statistiques agrégées, jamais une preuve.**
Aucun contenu réel n'a été inspecté (le pack n'en contient pas).

## Méthode et ses limites

Deux itérations de seuils ont été nécessaires (calibration empirique documentée dans le code du script) :
1. Une première passe avec des seuils absolus arbitraires a classé 80 % des régions en "table de texte"
   — le seuil (≥15 chaînes/région) était sous la médiane réelle (116 chaînes/région), donc non
   discriminant.
2. Seconde passe : seuils recalés sur les percentiles empiriques (p75/p90) de chaque signal, calculés sur
   les 2048 régions elles-mêmes. Distribution obtenue :

| Classification | Régions | Part |
|---|---|---|
| `zone_compressee_possible` | 711 | 34,7 % |
| `zone_indeterminee` | 570 | 27,8 % |
| `table_texte_probable` | 208 | 10,2 % |
| `table_pointeurs_probable` | 194 | 9,5 % |
| `structures_repetees_possible` | 170 | 8,3 % |
| `zone_texte_possible` | 129 | 6,3 % |
| `padding_probable` | 65 | 3,2 % |
| `code_thumb_probable` | 1 | 0,05 % |

**Observation importante** : `code_thumb_probable` ne ressort presque jamais avec les seuils actuels,
alors qu'un jeu complet contient nécessairement des centaines de Ko à plusieurs Mo de code. Ce n'est pas
une découverte ("Unbound a peu de code") mais une limite de méthode : le signal `thumb_bl_target_count`
est présent à un niveau non nul dans 2044/2048 régions (seulement 4 régions à zéro), donc lui aussi
manque de contraste net entre code et non-code avec ces seuils. **HYPOTHÈSE, confiance faible** sur
la localisation précise du code sans désassemblage réel.

## Régions ancres (déduites de conventions GBA publiques, pas du pack)

- **Région 0 (0x000000–0x003FFF)** : contient obligatoirement, par convention GBA standard (CONFIRMÉ,
  connaissance publique), l'en-tête de 192 octets et le tout début du code exécutable (vecteurs
  d'interruption puis code de démarrage). Profil mesuré : entropie 6.56, 359 signatures de compression
  candidates, 114 cibles Thumb, 2804 en cumul de références de pointeurs — classée
  `table_pointeurs_probable` (confiance 85) par notre heuristique, ce qui est **cohérent mais incomplet** :
  cette région est réellement un mélange en-tête + code + probablement une ou plusieurs tables de
  vecteurs/fonctions, pas une simple "table de pointeurs". Sert de point d'ancrage pour calibrer ce à
  quoi ressemble une zone "début de ROM" dans ce jeu de données.

## Candidats les plus solides par catégorie

### Tables de texte probables (top confiance)
| Région | Offset | Confiance | Entropie | Chaînes | Notes |
|---|---|---|---|---|---|
| 1128 | 0x11A0000–0x11A3FFF | 81 | 7.72 | 970 | Entropie très haute + fort volume de chaînes : compatible avec du texte compressé ou un index dense |
| 1119 | 0x117C000–0x117FFFF | 70 | 7.80 | 742 | |
| 1312 | 0x1480000–0x1483FFF | 69 | 7.76 | 741 | |

Ces trois régions sont proches les unes des autres en gamme d'offset (~0x117C000–0x1483FFF), suggérant
une **zone étendue candidate pour des tables de texte**, pas trois zones isolées. HYPOTHÈSE.

### Tables de pointeurs probables (top confiance, hors région 0)
| Région | Offset | Confiance | ptr_ref_sum | Notes |
|---|---|---|---|---|
| 140 | 0x230000–0x233FFF | 85 | 3007 | Entropie basse (3.89), 394 blocs répétés — compatible avec une table structurée dense, pas du texte |
| 232 | 0x3A0000–0x3A3FFF | 85 | 2447 | Aucune chaîne détectée dans la région — cohérent avec des données pures, pas du texte |
| 249 | 0x3E4000–0x3E7FFF | 82 | 2133 | |

Les régions 140, 232, 249 forment un groupe rapproché (~0x230000–0x3E7FFF). HYPOTHÈSE : zone candidate
pour des tables de données structurées (dresseurs, espèces, objets — voir `pokemon-data.md`), cohérent
avec l'emplacement habituel de ces tables tôt dans les ROM Pokémon GBA (juste après le code principal).

### Zones à haute entropie / compression possible
711 régions (34,7 % de la ROM) portent le label `zone_compressee_possible`, avec une confiance
volontairement plafonnée à 30 (signal jugé peu fiable, voir `compression.md`). C'est la catégorie la
plus représentée — cohérent avec le fait qu'une ROM Pokémon GBA est majoritairement constituée de
graphismes et de tuiles compressés (fait public bien établi sur ces jeux), mais on ne peut pas confirmer
plus précisément avec ces données seules.

### Padding probable
65 régions (3,2 %), concentrées presque exclusivement dans la plage ~0x164000–0x1AFFFF (13 des 65
listées dans le top). HYPOTHÈSE : fin de banque ou espace non utilisé/réservé dans cette zone.

## Fichier complet

`data/region-profile.csv` — 2048 lignes, colonnes : `region, offset_start, offset_end, entropy,
zero_ratio, ff_ratio, comp_sig_count, comp_lz77, comp_huffman, comp_rle, ptr_target_count, ptr_ref_sum,
string_count, string_bytes, repeated_block_count, arm_bl_target_count, thumb_bl_target_count,
classification, confidence`.
