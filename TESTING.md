# Tests en jeu — Pokémon Odyssey FR

Ce fichier suit la règle n°12 du mandat : la traduction n'est pas "terminée" simplement parce que les scripts se compilent, elle doit être vérifiée en jeu.

## Test prioritaire à faire en premier : le jeu de caractères

**Fichier :** `build/Pokemon_Odyssey_CHARTEST.gba` (généré par `tools/build_charset_test.py`, non commité au dépôt — voir `.gitignore`)

**Pourquoi :** seul le caractère `é` (et `Ä Ö Ü ä ö ü`) est confirmé dans la table de caractères de cette ROM (voir `docs/TECHNICAL_AUDIT.md` section 3). Tous les autres accents français (`à â ç è ê ë î ï ô ù û œ` + majuscules) reposent sur une hypothèse non vérifiée, faute d'émulateur avec affichage dans l'environnement où ce travail a été fait.

**Comment tester :**
1. Charger `build/Pokemon_Odyssey_CHARTEST.gba` dans n'importe quel émulateur GBA (mGBA, VBA, etc.) — la BIOS GBA n'est pas nécessaire pour ce test.
2. Depuis l'écran-titre, démarrer une nouvelle partie (ou continuer, peu importe).
3. Ouvrir le menu Start (bouton Start), choisir **SAVE / SAUVEGARDER**.
4. Lire le texte affiché : il doit dire `TEST CHARSET:` suivi de deux lignes montrant `a e i o u c : à â ç è ê ë î ï ô ù û` puis les majuscules `À Â Ç È Ê Ë Î Ï Ô Ù Û Œ œ`.

**Résultat attendu et action à suivre :**
| Observation | Signification | Action |
|---|---|---|
| Toutes les lettres accentuées s'affichent correctement | L'hypothèse basse-plage (`tools/build_charset_test.py:HYPOTHESIS_LOWRANGE`) est confirmée | Copier ces valeurs dans `CHARMAP` de `tools/gen3_charmap.py`, relancer `tools/build_french_rom.py` — toutes les traductions passeront automatiquement des accents en mode ASCII (ex. `a`) aux vrais accents |
| Certaines lettres sont des tuiles vides/du bruit graphique | Ces bytes précis sont faux ou non alloués à des glyphes dans cette ROM | Documenter précisément lesquelles dans ce fichier (ligne "bug"), il faudra une analyse plus poussée (désassemblage du moteur de rendu de texte) |
| Le jeu plante / freeze à l'ouverture du menu SAVE | Un des bytes utilisés entre en conflit avec un vrai code de contrôle du moteur | Ne pas réessayer sans investigation — signaler précisément quel(s) octet(s) sont en cause |

## Suivi des tests

| zone | événement | test | résultat | bug | correction |
|---|---|---|---|---|---|
| Save menu | Prompt de sauvegarde | Jeu de caractères accentués (voir ci-dessus) | **NON TESTÉ** (nécessite un émulateur avec affichage — indisponible dans cet environnement) | — | — |
| LOT 1 (système) | 63 chaînes système/UI traduites (save, wireless, mystery gift, union room, options...) | Build produit et validé structurellement (`tools/validate_rom.py`) mais **pas encore rejoué en jeu** | Construction OK, affichage réel non vérifié | — | — |

## Comment produire un build à tester

```
python3 tools/build_french_rom.py       # build/Pokemon_Odyssey_FR.gba (traductions validées, statut TRANSLATED+)
python3 tools/build_charset_test.py     # build/Pokemon_Odyssey_CHARTEST.gba (test dédié accents)
python3 tools/validate_rom.py           # vérifie taille/header/checksum du dernier build FR
```

Les deux ROM produites (`build/*.gba`) ne sont **pas commitées au dépôt git** (droits d'auteur — voir `.gitignore`) : elles doivent être régénérées localement à partir de `roms/original/odyssey_en_v4.1.1.gba` (également non commité) + `translation/text_database.tsv`.
