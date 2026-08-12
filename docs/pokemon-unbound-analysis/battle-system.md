# Moteur de combat

## Ce que le pack permet de dire : rien de spécifique

Aucune structure du pack n'est identifiable comme appartenant spécifiquement au moteur de combat (calcul
de dégâts, IA, gestion de statuts). Les seuls éléments indirectement pertinents :

- La cible Thumb la plus référencée (`0x0003FBE8`, 1345 références, voir `code-analysis.md`) pourrait en
  théorie être une fonction centrale du moteur de combat (le calcul de dégâts est typiquement appelé très
  fréquemment) — mais c'est une hypothèse parmi plusieurs possibles (interpréteur de script, fonction
  mémoire générique), non départageable avec ces données. **HYPOTHÈSE, confiance très faible.**
- La table candidate "attaques" (32 octets × 136, voir `pokemon-data.md`) est le seul lien indirect avec
  les données consommées par le moteur de combat, elle-même déjà qualifiée de confiance faible.

Aucune conclusion n'est possible sur : calcul de dégâts, gestion des types/efficacités, précision,
critiques, statuts, IA adverse, changements de statistiques, objets tenus en combat, animations,
transitions, ou logique des tours. **INCONNU pour l'intégralité de ces points.**

## Référence méthodologique publique

Le moteur de combat de FireRed (et par héritage architectural probable, celui d'Unbound/CFRU) est
documenté publiquement par `pret/pokefirered`, avec la même architecture générale que celle de notre
propre projet (`pokeemerald-expansion`, qui documente et implémente déjà un moteur de combat complet et
fonctionnel — mécaniques modernes actives, cf. `docs/TECHNICAL_ARCHITECTURE.md`). Pour toute question de
conception sur le combat, notre propre moteur déjà en place est la référence directement exploitable, pas
une inférence non vérifiable depuis ce pack statistique.

## Conclusion

Cette section ne peut pas être enrichie sans le contenu réel de la ROM. Rien dans project-recommendations
ne devrait se fonder sur une prétendue "découverte" du moteur de combat d'Unbound à partir de ce pack —
ce serait une hypothèse habillée en fait, contraire à la règle de rigueur demandée.
