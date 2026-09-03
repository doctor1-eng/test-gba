# Données de jeu (Pokémon, attaques, objets, dresseurs)

## Méthode

Cette section s'appuie uniquement sur `pointer-analysis.md` (séquences à pas constant parmi les cibles
de pointeurs) et `rom-map.md` (régions à forte densité de pointeurs). Aucun contenu réel n'a été inspecté
— toutes les identifications de système sont des HYPOTHÈSES basées sur la coïncidence entre taille de
struct observée et conventions publiques pret/pokefirered, jamais une confirmation.

## Candidats par système

### Espèces (base stats)
| Offset | Pas | Enregistrements | Hypothèse |
|---|---|---|---|
| `0x34F188` | 28 octets | 294 | Coïncide avec la taille historique du struct `BaseStats` dans les décompilations pret (générations pré-extension). Confiance : HYPOTHÈSE, moyenne. |
| `0xEBBAEC` | 36 octets | 298 | Struct plus large — pourrait correspondre à une version étendue du struct espèce (ajout d'items tenus, ratio de genre étendu, etc., pratique courante dans les hacks "expansion"-style). Confiance : HYPOTHÈSE, moyenne. |

298 et 294 enregistrements sont dans l'ordre de grandeur d'un Pokédex étendu (au-delà des 386-411
espèces de base FireRed/Émeraude selon les mods), mais rien ne confirme qu'il s'agit bien de ce système
plutôt que d'une coïncidence de stride sur des données non liées.

### Attaques
| Offset | Pas | Enregistrements | Hypothèse |
|---|---|---|---|
| `0xEBAA0C` | 32 octets | 136 | 32 octets est une taille plausible pour un struct "attaque" (puissance, précision, PP, type, catégorie, effet, cible, chance d'effet secondaire tiennent dans cet ordre de grandeur dans les conventions pret). 136 enregistrements est bien en dessous du nombre total d'attaques connu publiquement pour un Pokédex complet (600+) — soit une sous-table partielle, soit une coïncidence sans rapport. Confiance : HYPOTHÈSE, faible. |

### Objets, dresseurs
Aucun candidat de taille de struct suffisamment distinctif n'a émergé de `detect_tables.py` pour ces deux
catégories spécifiquement — les strides de 12, 20, 44 octets présents dans `data/table-candidates.md`
sont compatibles avec plusieurs systèmes différents (objets ET dresseurs ET beaucoup d'autres structures
de jeu partagent des tailles similaires dans ces conventions), donc non attribuables sans ambiguïté.
**INCONNU** pour ces deux catégories avec ce niveau de données.

### Progression (badges, flags, variables)
**INCONNU.** Aucun signal dans ce pack ne permet de distinguer une table de flags/variables d'une autre
structure de données — ce sont typiquement de petites valeurs numériques dans les saveblocks, pas des
structures identifiables par les heuristiques fournies (pointeurs, entropie, chaînes).

## Pourquoi ces hypothèses restent faibles

Le pack ne fournit aucun moyen de valider le CONTENU des enregistrements candidats (seuls offset/pas/
nombre sont connus). Une coïncidence de taille avec une convention publique est un indice, pas une
preuve — beaucoup de structures de données différentes dans un jeu compilé peuvent partager la même
taille par hasard (alignement mémoire, choix de compilation). Sans lecture d'au moins quelques
enregistrements bruts, aucune de ces hypothèses ne peut passer au niveau PROBABLE.
