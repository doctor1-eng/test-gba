# Pokémon Odyssey — traduction française

Chantier de traduction française de la ROM GBA *Pokémon Odyssey* (hack de Pokémon FireRed, v4.1.1 EN). Voir `docs/PROGRESS.md` pour l'état réel d'avancement et `docs/TECHNICAL_AUDIT.md` pour l'analyse technique de la ROM.

## Important — pas de ROM dans ce dépôt

Les fichiers `.gba` (original et build FR) ne sont **jamais commités** (droits d'auteur — voir `.gitignore`). Pour travailler localement :

1. Placer la ROM anglaise originale dans `roms/original/odyssey_en_v4.1.1.gba`.
   SHA-256 attendu : `44c7e3eafab19c39df7c39d54bafb78a1d9caf7c371244b6f5efb12cfd98d0d0`
2. `python3 tools/extract_text.py` régénère `translation/text_database.tsv` si besoin (déjà généré et versionné).
3. Éditer les traductions dans `translation/text_database.tsv` (colonne `french`, statut `TRANSLATED`).
4. `python3 tools/validate_text.py` — vérifie les codes de contrôle et l'encodabilité.
5. `python3 tools/build_french_rom.py` — produit `build/Pokemon_Odyssey_FR.gba` (local uniquement).
6. `python3 tools/validate_rom.py` — vérifie l'intégrité structurelle du build.

## Structure

- `docs/TECHNICAL_AUDIT.md` — audit technique de la ROM (pointeurs, table de caractères, codes de contrôle, espace libre).
- `docs/PROGRESS.md` — avancement réel, chiffré.
- `TESTING.md` — procédure de test en jeu, y compris le test de caractères accentués prioritaire.
- `translation/text_database.tsv` — base de toutes les chaînes extraites (ID stable `ODYSSEY-TXT-NNNNNN`).
- `translation/glossary.tsv` — cohérence terminologique.
- `tools/` — pipeline complet (extraction, validation, réinsertion), tous reproductibles en ligne de commande.
