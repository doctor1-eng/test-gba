# Registre de confiance

Toutes les affirmations de cette analyse, classées. Aucune n'est présentée ailleurs avec un niveau
supérieur à celui indiqué ici.

## CONFIRMÉ

| Affirmation | Source |
|---|---|
| Le pack ne contient aucune ROM, image, ni fichier audio — uniquement CSV/JSON/MD | Inspection directe de l'archive (`unzip -l`, lecture des en-têtes de fichiers) |
| Taille source déclarée : 33 554 432 octets | `manifest.json` (déclaratif) |
| En-tête GBA déclaré : titre `POKEMON FIRE`, code jeu `BPRE`, checksum valide | `manifest.json`/`deep-analysis.md` (déclaratif) |
| `BPRE` est le code jeu public de Pokémon FireRed (US) | Connaissance publique indépendante du pack |
| Comptages bruts fournis (307 109 chaînes, 197 543 pointeurs, etc.) | Lecture directe des CSV (`wc -l`) |
| `arm_bl_target_count >= 69` sur 100 % des 2048 régions | Calcul direct sur `region-profile.csv` |
| Médiane de longueur de chaîne = 5 octets sur 307 109 entrées | Calcul direct sur `string-index.csv` |
| Les jeux Pokémon GBA n'encodent pas le texte en ASCII standard (charmap propriétaire) | Connaissance publique + notre propre `engine/charmap.txt` dans ce projet |

## PROBABLE

| Affirmation | Justification |
|---|---|
| Unbound est bâti sur une base FireRed (pas Émeraude) | `BPRE` confirmé + convergence avec notre propre `PROJECT_ANALYSIS.md` (Session 1), deux sources indépendantes |
| Le signal `arm-bl-targets.csv` est dominé par des faux positifs | Couverture universelle + absence de fonction "hub" (max 13 réf.) + contraste avec Thumb (max 1345 réf.) |
| Le signal `compression-signatures.csv` a un taux de faux positifs significatif | Présence dense (359) dans la région 0, structurellement non-compressée par convention GBA |
| `string-index.csv` ne représente très majoritairement pas le texte de dialogue réel | Encodage charmap vs scanner ASCII générique + distribution de longueur incompatible avec du dialogue |

## HYPOTHÈSE

| Affirmation | Niveau |
|---|---|
| Régions 1128/1119/1312 (haute entropie + beaucoup de chaînes courtes) = données compressées/binaires denses | Moyenne — reclassification suggérée dans `dialogues.md` |
| Régions 140/232/249 = tables de données structurées | Moyenne — cohérent avec position typique post-code dans les ROM Pokémon GBA, non vérifié |
| Séquence 0x34F188 (28 octets × 294) = table d'espèces | Moyenne — coïncidence de taille de struct avec convention pret |
| Séquence 0xEBBAEC (36 octets × 298) = table d'espèces étendue | Moyenne — idem |
| Séquence 0xEBAA0C (32 octets × 136) = table d'attaques | Faible — nombre d'enregistrements trop bas pour un Pokédex complet |
| Cible Thumb `0x0003FBE8` (1345 réf.) = fonction utilitaire centrale | Faible — identité précise indéterminable |
| Cible Thumb `0x00000A38` = liée au code de démarrage | Faible — position seule, pas de contenu vérifié |

## INCONNU

- Tout ce qui touche au contenu réel : dialogues, noms, graphismes, musique, scénario d'Unbound.
- Système de cartes, de scripts, de combat en détail (structures précises, pas seulement "où
  probablement").
- Toute extension propriétaire spécifique à Unbound au-delà des conventions FireRed/CFRU publiques.
- Version/révision exacte au-delà de l'octet `version: 0` standard.
- Validité réelle du SHA-256 déclaré (non recalculable sans le binaire).

## Ce que "ne jamais inventer" a concrètement changé dans cette analyse

- La classification `table_texte_probable` a été explicitement remise en question et requalifiée dans
  `dialogues.md` plutôt que d'être prise pour argent comptant malgré son nom.
- Le signal ARM a été retiré de la classification automatique après découverte de sa saturation, plutôt
  que gardé avec un poids arbitraire.
- `maps.md`, `scripts.md`, `battle-system.md` déclarent explicitement l'absence de données exploitables
  plutôt que de combler avec des généralités présentées comme spécifiques à Unbound.
