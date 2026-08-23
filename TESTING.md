# Tests en jeu — Pokémon Odyssey FR

Ce fichier suit la règle n°12 du mandat : la traduction n'est pas "terminée" simplement parce que les scripts se compilent, elle doit être vérifiée en jeu.

## Mise à jour majeure (2026-08-23) : test réel en émulateur maintenant possible sans affichage

`mgba-sdl` sous Xvfb s'est révélé inutilisable dans cet environnement (fenêtre créée, focus détecté, mais le cœur d'émulation ne rendait jamais rien — même la ROM anglaise d'origine restait à l'écran noir ; limitation de cet environnement, pas un bug du jeu). Les vraies liaisons Python de mGBA (`pip install mgba`) contournent complètement le problème : elles pilotent le cœur d'émulation directement en mémoire, sans SDL ni serveur d'affichage, et exposent le framebuffer brut.

**Outils** (voir aussi `requirements-test.txt`) :
- `tools/headless_playtest.py <rom.gba> <out_dir> [n_presses]` — boot la ROM, laisse jouer les logos, puis mitraille le bouton A (+ Start périodiquement) pour avancer dans les boîtes de dialogue, en sauvegardant une capture d'écran PNG et un indice de variance (`std`) à intervalles réguliers. Comparer deux runs (ex. ROM anglaise vs ROM française, avec la même séquence de touches) permet de repérer automatiquement un écran corrompu (variance qui s'effondre près de 0 = image en une seule couleur unie) sans jamais avoir besoin d'un vrai appareil.
- `tools/replay_patch_log.py <n> <out.gba> [id_a_exclure ...]` — reconstruit une ROM en ne rejouant que les N premières lignes de `build/patch_log.tsv` (dans l'ordre), avec la possibilité d'exclure des IDs précis. Combiné avec `headless_playtest.py`, ça permet une **bissection automatique** : trouver exactement quelle ligne de traduction casse l'affichage, sans deviner à partir d'une analyse statique seule.

C'est exactement cette méthode (bissection + comparaison EN/FR automatisée) qui a permis de confirmer et corriger le bug de corruption de l'écran-titre du 2026-08-23 (voir `docs/PROGRESS.md`) — la ligne fautive (`ODYSSEY-TXT-004785`, une référence unique et jamais validée pointant en plein dans le code de démarrage du ROM à l'adresse `0x0000D90`) a été isolée automatiquement en une douzaine d'itérations de recherche dichotomique, puis confirmée visuellement (capture d'écran de l'écran-titre correctement restauré) — pas seulement déduite par raisonnement.

**Usage rapide :**
```
pip install -r requirements-test.txt
python3 tools/headless_playtest.py roms/original/odyssey_en_v4.1.1.gba /tmp/en_test 150
python3 tools/headless_playtest.py build/Pokemon_Odyssey_FR.gba /tmp/fr_test 150
# comparer /tmp/en_test/*.png et /tmp/fr_test/*.png visuellement, ou les colonnes "std" imprimées
```

## Test du jeu de caractères — accents français

**Historique :** le test dédié `build/Pokemon_Odyssey_CHARTEST.gba` (généré par `tools/build_charset_test.py`) a été confirmé visuellement par l'utilisateur sur un émulateur réel le 2026-08-22 pour 24 des 26 caractères testés. **Bug trouvé le 2026-08-23** : la chaîne de test elle-même omettait `é` et `É` — ces deux caractères n'ont donc jamais été réellement vus à l'écran malgré leur promotion en "confirmé". Corrigé (voir `docs/PROGRESS.md` et `docs/TECHNICAL_AUDIT.md`) : `é` retombe sur l'octet `0xF7`, authentiquement confirmé par ailleurs (présent dans « Pokémon » en texte ROM lisible) ; `É` n'a pas d'octet confirmé disponible et retombe sur `E` sans accent.

## Suivi des tests

| zone | événement | test | résultat | bug | correction |
|---|---|---|---|---|---|
| Jeu de caractères (24/26 lettres) | Menu Start > SAVE | Chaîne de test dédiée sur émulateur réel | **Confirmé** par l'utilisateur (2026-08-22) | `é`/`É` absents de la chaîne de test, promus "confirmé" par erreur | Voir ci-dessus (2026-08-23) |
| Écran-titre | Boot du jeu | `tools/headless_playtest.py` (mGBA headless, comparaison EN/FR) | **Confirmé visuellement** — logo "Pokémon Odyssey" identique à l'original (2026-08-23) | Référence unique non validée (`0x0000D90`, code de boot) corrompant le titre | `tools/build_french_rom.py`: filtre plancher d'adresse plausible (`MIN_PLAUSIBLE_REF_ADDR`) |
| Narration d'introduction | Après l'écran-titre | `tools/headless_playtest.py`, 150 points de contrôle comparés EN/FR | **Confirmé visuellement** — texte français lisible sur fond graphique correct | Fragments qui se chevauchent avec références ambiguës (voir PROGRESS.md) | Mode strict de `trustworthy_refs()` pour les lignes `overlapping_ids` |
| Reste du jeu (menus, combats, dialogues NPC) | — | Variance std comparée EN/FR sur 150 points de contrôle | Cohérent (pas d'effondrement vers une couleur unie) | — | — |

## Limitation honnête découverte le 2026-08-23

Le test approfondi a révélé au moins 2 fragments de texte (dans la narration d'introduction) accessibles uniquement via une adresse **calculée/indirecte**, invisible pour `tools/extract_text.py` qui ne scanne que les pointeurs absolus 4 octets alignés. Ces fragments restent donc en anglais dans le build — ce n'est pas une corruption, seulement une lacune de couverture déjà documentée comme risque connu (`docs/TECHNICAL_AUDIT.md` section 9 : « une chaîne référencée par pointeur arithmétique... »), maintenant confirmée avec un exemple concret. Corriger ça demanderait de rétro-ingénierer le mécanisme d'adressage indirect du moteur de script — hors du périmètre de cette session.

## Comment produire un build à tester

```
python3 tools/build_french_rom.py       # build/Pokemon_Odyssey_FR.gba (traductions validées, statut TRANSLATED+)
python3 tools/build_charset_test.py     # build/Pokemon_Odyssey_CHARTEST.gba (test dédié accents)
python3 tools/validate_rom.py           # vérifie taille/header/checksum du dernier build FR
```

Les ROM produites (`build/*.gba`) ne sont **pas commitées au dépôt git** (droits d'auteur — voir `.gitignore`) : elles doivent être régénérées localement à partir de `roms/original/odyssey_en_v4.1.1.gba` (également non commité) + `translation/text_database.tsv`.
