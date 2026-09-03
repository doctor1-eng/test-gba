# Identification technique

Source : `manifest.json` et `deep-analysis.md` du pack d'analyse (déclaratifs, produits par l'outil
d'analyse statique de Thomas en dehors de cet environnement — nous n'avons pas recalculé ces valeurs
nous-mêmes, nous les rapportons telles que fournies).

## Faits déclarés par le pack

| Champ | Valeur | Statut |
|---|---|---|
| Taille source | 33 554 432 octets (32 MiB, 0x02000000) | CONFIRMÉ (déclaratif) |
| SHA-256 | `7aa25bbf568f7cfcf6ee1cf2e9e6ff637350b3d0705c2375cabb6baa7d9739f7` | CONFIRMÉ (déclaratif, non vérifiable sans le binaire) |
| Titre GBA (en-tête) | `POKEMON FIRE` | CONFIRMÉ (déclaratif) |
| Code jeu | `BPRE` | CONFIRMÉ (déclaratif) |
| Mainteneur | `01` | CONFIRMÉ (déclaratif) |
| Octet de version | `0` | CONFIRMÉ (déclaratif) |
| Somme de contrôle d'en-tête | `0x68`, déclarée valide | CONFIRMÉ (déclaratif) |

## Déductions à partir de ces faits

- **PROBABLE, confiance élevée** : le code jeu `BPRE` est publiquement et largement documenté (bases de
  données GBA communautaires, littérature ROM hacking) comme celui de *Pokémon FireRed (U)*. C'est un
  fait public indépendant du pack fourni, pas une déduction depuis les données elles-mêmes.
- **PROBABLE, confiance élevée** : Unbound est donc bâti sur une base FireRed, pas Émeraude. Ceci est
  cohérent avec ce que notre propre projet avait déjà noté indépendamment en Session 1
  (`docs/PROJECT_ANALYSIS.md`) : "Unbound tourne sur CFRU (Complete FireRed Upgrade, binaire, licence
  non-commerciale stricte)". Deux sources indépendantes convergent — c'est le niveau de confiance le plus
  solide de toute cette analyse.
- **HYPOTHÈSE** : la taille de 32 MiB (le maximum standard pour une cartouche GBA) suggère un jeu qui
  utilise la quasi-totalité de l'espace disponible, cohérent avec un hack ajoutant beaucoup de contenu à
  une base FireRed (16 MiB à l'origine). Non vérifiable sans mesurer l'occupation réelle vs le padding.

## Format et architecture cible

- **CONFIRMÉ (connaissance publique, pas dérivée du pack)** : architecture Game Boy Advance — ARM7TDMI,
  mode d'exécution mixte ARM (32 bits) / Thumb (16 bits), adressage ROM en `0x08000000`–`0x09FFFFFF`.
  Cohérent avec les préfixes d'adresse observés dans tous les CSV du pack (`0x08xxxxxx`/`0x09xxxxxx`).
- **INCONNU** : version précise de la région/du dump (le pack ne fournit pas de numéro de révision
  au-delà de l'octet `version: 0`, qui est standard pour la plupart des ROM GBA commerciales).
- **INCONNU** : présence d'un header additionnel spécifique à Unbound/CFRU au-delà de l'en-tête GBA
  standard (le pack ne documente que les 192 premiers octets d'en-tête GBA classiques).

## Ce que ce pack ne permet PAS de déterminer

- Le contenu réel de l'en-tête au-delà des champs listés (titre, code jeu, mainteneur, checksum) —
  le pack ne fournit pas de dump hexadécimal des 192 premiers octets.
- La présence de formats propriétaires spécifiques à Unbound (le pack ne fait pas de distinction entre
  "conventions FireRed standard" et "extensions propres à Unbound/CFRU").
- Toute signature de bibliothèque ou de compilateur (pas d'analyse de code réelle, seulement des
  candidats statistiques — voir `code-analysis.md`).
