# Système de cartes

## Ce que le pack permet réellement de dire : très peu

Le pack ne contient aucune structure identifiée spécifiquement comme "en-tête de carte", "tileset" ou
"connexion" — ces CSV sont génériques (pointeurs, entropie, chaînes, compression) et n'ont pas de
classification sémantique par système de jeu. Toute affirmation spécifique aux cartes serait une
extrapolation non fondée sur les données. **INCONNU** pour la quasi-totalité de cette section à partir du
pack seul.

Les seuls éléments exploitables sont indirects :
- Les régions classées `table_pointeurs_probable` (`rom-map.md`) sont des candidats génériques pour
  *toute* table de pointeurs du jeu, cartes incluses — mais rien ne permet de les distinguer d'une table
  de pointeurs d'un autre système (objets, attaques, dresseurs...).
- Les zones à haute entropie (`compression.md`) sont des candidats génériques pour des tuiles/tilesets
  compressés — même limite.

## Référence méthodologique publique (pas dérivée du pack, clairement distinguée)

Puisque Unbound est confirmé bâti sur une base FireRed (`technical-structure.md`), le projet public
`pret/pokefirered` (décompilation légale et documentée de FireRed/LeafGreen) documente publiquement les
conventions suivantes, qui s'appliquent *probablement* à Unbound par héritage architectural — mais ceci
reste une hypothèse par analogie, jamais vérifiée sur les données réelles du pack :

- Cartes définies par un en-tête (`MapHeader`) référençant un `MapLayout` (dimensions, tileset primaire,
  tileset secondaire, données de tuiles/collisions) et des tableaux d'événements (objets, warps,
  déclencheurs de coordonnées, panneaux).
- Tilesets composés de tuiles 4bpp compressées (souvent LZ77) + palette + metatiles (association de
  4 tuiles + propriétés de collision/comportement).
- Connexions entre cartes stockées comme une liste par carte (direction, décalage, carte cible).

C'est exactly la même architecture que celle qu'on utilise déjà dans **notre propre projet**
(`pokeemerald-expansion`, `data/maps/*/map.json`) — donc si l'objectif est de s'inspirer de conventions
de structure de cartes, la référence la plus fiable et actionnable reste directement notre propre moteur
et `pret/pokefirered`, pas une inférence depuis ce pack statistique.

## Conclusion

Cette section ne peut pas être enrichie davantage sans accès au contenu réel de la ROM (que ce pack ne
fournit pas par conception). Voir `project-recommendations.md` : la recommandation pour tout ce qui
touche aux cartes est de s'appuyer sur nos propres conventions déjà en place, pas sur une rétro-ingénierie
d'Unbound.
