# Suivi de progression — traduction française Pokémon Odyssey

Dernière mise à jour : 2026-08-22

## État réel (pas une estimation arrondie)

| Métrique | Valeur |
|---|---|
| Chaînes extraites (texte pointé, base) | **6 830** |
| … dont bruit de décodage (espaces/fragments non exploitables) | 521 (marquées `SKIP_NOISE`, conservées, jamais supprimées) |
| Chaînes de texte réel restant à traiter | 6 246 |
| Traduites (`TRANSLATED`) | **63** (LOT 1 — interface/système) |
| Validées structurellement (contrôle des codes, encodage) | 63 / 63, 0 erreur |
| Réinsérées dans un build ROM | 63 / 63 (`build/Pokemon_Odyssey_FR.gba`) |
| Testées en jeu (affichage réel vérifié) | **0** — aucun test visuel possible dans cet environnement (pas d'émulateur avec affichage) |

**Pourcentage global de traduction : ~1 % (63 / 6 309 chaînes réelles utiles).** Ne pas arrondir vers le haut.

## Ce qui est fait et vérifié

- Audit technique complet (`docs/TECHNICAL_AUDIT.md`) : ROM identifiée (hack de FireRed BPRE, 32 Mio), pointeurs GBA standards confirmés empiriquement sur 5866+ occurrences, table de caractères confirmée pour `0x00`+`0xA1`-`0xF7` (chiffres, ponctuation, A-Z, a-z, é), terminateur `0xFF`, codes de contrôle `<FE> <FA> <FB> <FC:xx> <FD:xx>` identifiés avec exemples réels.
- 15,93 Mio d'espace libre repéré pour la relogation des chaînes plus longues (`tools/find_free_space.py`).
- Pipeline complet et fonctionnel, de bout en bout, testé sur le LOT 1 :
  extraction (`tools/extract_text.py`) → traduction (`translation/text_database.tsv`) → validation (`tools/validate_text.py`) → réinsertion (`tools/build_french_rom.py`) → validation ROM (`tools/validate_rom.py`).
- LOT 1 (interface et messages système — sauvegarde, Adaptateur Sans Fil, CADEAU MYSTÈRE, Union Room, enregistrement, options) : 63 chaînes traduites, validées, réinsérées. Build produit, taille/header/checksum OK, 3207 octets modifiés sur 32 Mio (0,01 %).

## Ce qui N'EST PAS fait (à ne pas prétendre terminé)

- **6 246 chaînes de texte réel restantes** (LOTs 2 à 8 du mandat : introduction/premiers événements, scénario principal, PNJ/villes, objets/boutiques, combats, quêtes secondaires, textes résiduels). C'est l'essentiel du volume du jeu (dialogues, Pokédex, noms de lieux, objets, capacités...).
- **Aucun test visuel en jeu.** `mgba-sdl` a été installé mais ne dispose ni d'affichage ni de scripting exploitable dans cet environnement headless — impossible de confirmer que le texte s'affiche correctement, que les boîtes de dialogue ne débordent pas, ou que les codes de contrôle se comportent comme prévu en jeu réel.
- **Caractères accentués français au-delà de `é` non confirmés** (`à â ç è ê ë î ï ô ù û œ` + majuscules). Un build de test dédié existe (`build/Pokemon_Odyssey_CHARTEST.gba`, voir `TESTING.md`) mais n'a pas pu être vérifié visuellement ici. Tant que non confirmé, le pipeline de production utilise un repli ASCII sûr (accents supprimés) — voir `tools/gen3_charmap.py`.
- Largeur exacte des boîtes de dialogue non mesurée (pas de règle dure imposée par le validateur, seulement un avertissement au-delà de 2x la longueur anglaise).
- Nombre exact de paramètres de chaque sous-code `0xFC xx` non déterminé précisément (actuellement décodé de façon conservative sur 1 paramètre).
- `translation/context_database.tsv` (carte/événement/flags par chaîne) non créé — la contextualisation précise par script/carte n'a pas été faite ; seul un tag de catégorie générique a été ajouté aux 63 chaînes du LOT 1.

## Prochaines étapes recommandées, dans l'ordre du mandat

1. Faire vérifier `build/Pokemon_Odyssey_CHARTEST.gba` par un humain avec un vrai émulateur (30 secondes, voir `TESTING.md`) — c'est bloquant avant toute traduction massive utilisant des accents autres que `é`.
2. LOT 2 : introduction et premiers événements.
3. LOT 3 : scénario principal (le plus gros volume).
4. Continuer lot par lot jusqu'au LOT 8, en revalidant (`validate_text.py`) et en reconstruisant (`build_french_rom.py`) à chaque lot.
5. Une fois un lot narratif conséquent traduit, tester réellement en jeu (émulateur avec affichage) et remplir le tableau de `TESTING.md`.
