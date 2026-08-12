# Analyse des pointeurs et tables candidates

## Limite structurelle importante

`pointer-candidates.csv` ne fournit que `(adresse_cible, nombre_de_références)` — c'est-à-dire, pour
chaque adresse cible candidate, combien de fois un mot de 4 octets ailleurs dans la ROM "pointe" vers
elle (heuristique d'alignement + plage d'adresse valide). **Il ne fournit pas la position des pointeurs
eux-mêmes.** On ne peut donc pas localiser un "en-tête de table de pointeurs" au sens strict (une
séquence de pointeurs consécutifs) avec ces données seules — seulement repérer des adresses cibles
individuellement référencées et régulièrement espacées, compatible avec un tableau de structures de
taille fixe chacune pointée depuis ailleurs (par exemple depuis une table séparée qu'on ne voit pas).

Généré par `tools/unbound_analysis/detect_tables.py`. Sortie complète : `data/table-candidates.md`
(2117 séquences à pas constant, 465 clusters de blocs répétés denses).

## Séquences les plus solides (par longueur)

| Offset | Pas | Longueur | Taille totale | Confiance | Notes |
|---|---|---|---|---|---|
| `0xEBBAEC` | 36 octets | 298 | 0x29E8 (~10,6 Ko) | HYPOTHÈSE, moyenne | 298 enregistrements est un ordre de grandeur plausible pour une table d'espèces Pokémon (national dex ~300-400 selon génération incluse) ; 36 octets est dans la fourchette plausible d'un struct "espèce" étendu (les structs `BaseStats` des projets pret dépassent 28 octets une fois enrichis). **Non confirmé** — coïncidence de taille, pas de contenu inspecté. |
| `0x34F188` | 28 octets | 294 | 0x2028 (~8,2 Ko) | HYPOTHÈSE, moyenne | 28 octets correspond exactement à la taille historique du struct `BaseStats` dans les décompilations pret (générations plus anciennes, avant extensions). 294 enregistrements est proche de 298 ci-dessus — pourrait être une table liée ou une coïncidence de stride. |
| `0x34F1A4` | 56 octets | 213 | 0x2E98 | HYPOTHÈSE, faible | 56 = 28×2 ; se chevauche partiellement avec la séquence précédente (artefact probable : un stride multiple d'un stride réel se redétecte automatiquement, pas une structure distincte) |
| `0x238E8C` | 4 octets | 149 | 0x254 | HYPOTHÈSE, faible | Stride de 4 octets = soit une table de pointeurs pure (peu probable vu qu'il s'agit ici de cibles, pas de sources), soit une coïncidence sur des données courtes non structurées |
| `0xEBAA0C` | 32 octets | 136 | 0x1100 | HYPOTHÈSE, faible | 32 octets est une taille de struct plausible pour un enregistrement "attaque" (`puissance, précision, PP, type, catégorie...` tient dans 32 octets dans les conventions pret) |

**Aucune de ces hypothèses n'est vérifiable sans inspection du contenu réel** (que ce pack ne fournit
pas). Elles sont rapportées parce que la convergence taille-de-struct/nombre-d'enregistrements avec les
conventions publiques pret/pokefirered est suggestive, pas parce qu'elle est démontrée.

## Clusters denses de blocs de 16 octets répétés

465 clusters détectés (≥20 blocs déjà-vus dans une fenêtre de 16 Ko). Les plus denses se recoupent avec
les régions déjà classées `structures_repetees_possible` ou `padding_probable` dans `rom-map.md` — utile
comme signal croisé, mais rappel : un cluster dense de blocs répétés peut aussi bien signifier "table de
structures similaires" (ex. dresseurs avec des champs souvent identiques) que "remplissage" (0xFF/0x00
répété). La distinction se fait via `zero_ratio`/`ff_ratio` dans `region-profile.csv`, déjà appliquée
dans la classification de `rom-map.md`.

## Recommandation méthodologique pour aller plus loin

Pour transformer ces HYPOTHÈSES en PROBABLE, il faudrait au minimum :
1. Les octets bruts autour de `0xEBBAEC`/`0x34F188` pour vérifier si les enregistrements contiennent des
   valeurs plausibles de statistiques Pokémon (bytes 1-255 dans une plage cohérente, pas du texte ASCII).
2. Une table de pointeurs source pointant séquentiellement vers ces cibles, pour confirmer qu'il s'agit
   bien d'un tableau adressé par index (signature classique d'une table de données de jeu).

Aucune de ces deux vérifications n'est possible avec le pack fourni (métadonnées seules).
