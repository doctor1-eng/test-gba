# Système de scripts

## Ce que le pack permet réellement de dire

Rien de spécifique. Le pack ne contient aucune table de commandes, aucun bytecode identifié, aucune
structure classée "script". Les candidats génériques (`table_pointeurs_probable`, cibles Thumb très
référencées dans `code-analysis.md`) pourraient en théorie inclure l'interpréteur de script du jeu (une
fonction "exécuter la commande suivante" serait statistiquement très référencée, ce qui correspondrait au
profil de la cible Thumb `0x0003FBE8`, 1345 références) — mais c'est une coïncidence de profil parmi
plusieurs hypothèses possibles (voir `code-analysis.md`), pas une identification. **HYPOTHÈSE, confiance
très faible.**

## Référence méthodologique publique (par analogie architecturale, pas vérifiée sur le pack)

FireRed (et par héritage, très probablement Unbound/CFRU) utilise publiquement un moteur de script à
bytecode documenté par `pret/pokefirered` : une table d'opcodes 1 octet, chacun avec un nombre fixe
d'opérandes, interprétés par une boucle centrale. Commandes typiques documentées publiquement : affichage
de message, lecture/écriture de variables et flags, sauts conditionnels, appels de fonction native,
déplacement de PNJ, déclenchement de combat. C'est exactement le même modèle que celui qu'on utilise déjà
dans notre projet (`data/scripts/*.inc`, compilé via le préprocesseur de script de pokeemerald-expansion).

**Rien dans ce pack ne permet de confirmer si Unbound utilise ce même bytecode tel quel, une version
étendue, ou un système différent.** CFRU est connu publiquement pour ajouter des commandes de script
personnalisées au-delà de la base FireRed (mécaniques de repousse-suiveur, follower Pokémon, etc.) — mais
cette information vient de la réputation publique du projet CFRU, pas d'une déduction depuis le pack.

## Conclusion

Comme pour `maps.md`, cette section ne peut pas être enrichie sans le contenu réel de la ROM. La
recommandation pratique (`project-recommendations.md`) est la même : notre propre moteur de script
(hérité de pokeemerald-expansion, déjà utilisé et validé dans ce projet) est la référence directement
exploitable, pas une rétro-ingénierie hypothétique du bytecode d'Unbound.
