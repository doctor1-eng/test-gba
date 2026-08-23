# Audit technique — Pokémon Odyssey (English) v4.1.1

Date : 2026-08-22
ROM analysée : `roms/original/odyssey_en_v4.1.1.gba` (jamais modifiée — voir `.gitignore`, le binaire n'est pas commité dans le dépôt pour des raisons de droits d'auteur)

## 1. Identité de la ROM

| Champ | Valeur |
|---|---|
| Taille fichier | 33 554 432 octets (32 MiB, 0x02000000) |
| SHA-256 | `44c7e3eafab19c39df7c39d54bafb78a1d9caf7c371244b6f5efb12cfd98d0d0` |
| Titre header | `POKEMON FIRE` |
| Game code | `BPRE` |
| Maker code | `01` |
| Version logicielle | 0 |
| Complement check | `0x68` — **valide** (recalculé et vérifié) |
| Logo Nintendo | présent |

**Conclusion :** Pokémon Odyssey est un hack de **Pokémon FireRed (US, BPRE)**, étendu de 16 MiB (ROM FireRed d'origine) à 32 MiB. C'est cohérent avec les outils communautaires standards (Advance Map / XSE / etc.) qui étendent la ROM et utilisent l'espace ajouté (`0x01000000`–`0x02000000`) comme zone libre pour les données du hack.

Outil : `tools/gba_header.py`

## 2. Format des pointeurs

GBA : pointeur 4 octets little-endian, `adresse_ROM = valeur - 0x08000000`. Pour une ROM de 32 MiB, l'octet de poids fort d'un pointeur valide est `0x08` (offset < 0x01000000) ou `0x09` (offset >= 0x01000000).

Confirmé empiriquement : `tools/scan_pointers.py` a trouvé **5866 pointeurs structurellement valides** (octet haut correct, cible dans la ROM) dont la cible se décode en texte anglais cohérent avec la table de caractères ci-dessous (dialogues, menus, noms). La double coïncidence pointeur-valide + texte-lisible est une preuve solide — impossible à obtenir par hasard sur autant d'occurrences.

## 3. Table de caractères (charmap)

**CONFIRMÉ empiriquement** (par décodage de ~5800 pointeurs vers du texte anglais cohérent) :

- `0x00` = espace
- `0xA1`–`0xAA` = chiffres `0`–`9`
- `0xAB` `!`, `0xAC` `?`, `0xAD` `.`, `0xAE` `-`, `0xAF` `·`, `0xB0` `…`, `0xB1` `“`, `0xB2` `”`, `0xB3` `‘`, `0xB4` `’`, `0xB5` `♂`, `0xB6` `♀`, `0xB7` `$`, `0xB8` `,`, `0xB9` `×`, `0xBA` `/`
- `0xBB`–`0xD4` = `A`–`Z`
- `0xD5`–`0xEE` = `a`–`z`
- `0xF0` `:`, `0xF1` `Ä`, `0xF2` `Ö`, `0xF3` `Ü`, `0xF4` `ä`, `0xF5` `ö`, `0xF6` `ü`, `0xF7` `é` (confirmé, utilisé dans « Pokémon »)
- `0xFF` = **terminateur de chaîne**

Table : `tools/gen3_charmap.py`. Chaque entrée y est commentée comme confirmée.

**MISE À JOUR 2026-08-22 — CONFIRMÉ :** l'hypothèse ci-dessous a été testée avec `build/Pokemon_Odyssey_CHARTEST.gba` sur un émulateur réel (menu Start > SAVE) et validée par l'utilisateur. La table basse (`0x01`–`0x28`) est désormais intégrée à `CHARMAP` dans `tools/gen3_charmap.py` comme confirmée. Effet de bord découvert : l'ancien octet `0x01`, précédemment supposé être un "saut de ligne" (jamais vérifié), entrait en conflit avec `À` — l'hypothèse de saut de ligne a été retirée de `CONTROL_CODES` (elle n'était qu'une supposition non confirmée de toute façon). Une ré-extraction complète après ce correctif a fait passer la base de 6830 à 9051 chaînes uniques (2221 chaînes supplémentaires récupérées, auparavant rejetées à cause d'un octet accentué non reconnu qui cassait le décodage).

**CORRECTIF 2026-08-23 — cette confirmation était incomplète pour 2 des 26 caractères.** La chaîne réellement affichée à l'écran lors du test du 2026-08-22 (`"à â ç è ê ë î ï ô ù û"` / `"À Â Ç È Ê Ë Î Ï Ô Ù Û Œ œ"`) ne contient ni `é` ni `É` — ces deux lettres n'ont jamais été vues à l'écran, contrairement aux 24 autres qui l'ont bien été. `0x06='É'` et `0x1B='é'` avaient pourtant été inclus dans la promotion "CONFIRMÉ" ci-dessus par erreur d'inattention. Pire, `0x1B='é'` faisait doublon avec `0xF7='é'` (celui-là authentiquement confirmé, ligne 38 ci-dessus) et gagnait silencieusement la table inverse d'encodage utilisée pour tout le français — donc chaque `é` tapé par un traducteur (45,5 % des lignes traduites) a été écrit avec l'octet jamais vérifié `0x1B` plutôt que le `0xF7` confirmé. Trouvé lors d'un audit complet déclenché par un rapport utilisateur de corruption en jeu (voir `docs/PROGRESS.md`, section « Second bug »). Corrigé : `0x1B` retiré de `CHARMAP` (l'encodage retombe sur `0xF7`), `0x06` retiré (repli ASCII vers `E` sans accent, sans alternative confirmée disponible).

**Historique (avant confirmation) — risque initialement identifié :** la plage `0x01`–`0xA0` correspond, dans la table Gen III internationale standard largement documentée par la communauté (pret/pokefirered, Advance Text, etc.), aux lettres accentuées supplémentaires (`À Â Ç È Ê Ë Î Ï Ô Ù Û Ñ ß à â ç è ê ë î ï ô ù û ñ œ Œ` etc.) nécessaires au français au-delà de `é`. Cette plage n'apparaît dans **aucune** chaîne anglaise valide trouvée dans la ROM (normal, l'anglais ne les utilise pas), donc je n'ai **aucune preuve directe** qu'elle soit câblée de la même façon dans cette ROM précise.

Tentatives de vérification faites, sans résultat concluant :
1. Recherche de blocs LZ77 décompressés faisant exactement 256×64 octets (feuille de police complète, 1 glyphe par valeur de code 0x00-0xFF) → 3 candidats trouvés (`0x0D15FB8`, `0x0EA7320`, `0x102C46C`), rendus en image (`tools/render_font.py`) : aucun ne ressemble clairement à une grille de police lisible (probablement d'autres graphismes tuiles de même taille compressée).
2. Émulateur mGBA (`mgba-sdl`) installé mais son build ne propose pas de scripting Lua ni de mode capture d'écran headless en CLI dans cet environnement (pas de serveur d'affichage) — impossible de faire un test d'affichage réel en jeu depuis cette session.

**Conséquence pratique pour la suite :**
- Le décodeur (`decode_bytes`) échoue proprement (ok=False) sur tout octet `0x01`-`0xA0` non mappé, plutôt que de deviner — aucune traduction ne sera donc silencieusement corrompue par cette incertitude.
- La table hypothétique internationale standard sera utilisée comme **hypothèse de travail documentée**, mais toute chaîne française nécessitant un caractère accentué autre que `é/É/à` devra être marquée `REVIEW: charset non vérifié` tant qu'un test visuel réel (émulateur avec affichage) n'aura pas été fait par l'utilisateur ou dans un environnement disposant d'un affichage.
- Stratégie de repli recommandée : préférer autant que possible des tournures françaises correctes n'exigeant pas de caractère non confirmé (`à`, `é`, `è`, `ê`, `ç`, `ù`, `î`, apostrophe `’` déjà confirmée) ; **les accents `é/è` sur la plage confirmée + `à` (à vérifier en priorité, très fréquent en français) suffisent à la grande majorité du texte**.

## 4. Codes de contrôle identifiés

| Code | Rôle observé |
|---|---|
| `0xFF` | terminateur de chaîne (fin) |
| `0xFE` | nouveau paragraphe (efface la boîte, continue à imprimer) |
| `0xFA` | attend un appui bouton, garde la boîte affichée sans l'effacer |
| `0xFB` | attend un appui bouton puis efface/scrolle la boîte |
| `0xFC xx` | code spécial (couleur/police/pause musique...), au moins 1 octet de paramètre — **le nombre exact de paramètres varie selon le sous-code `xx`** dans le moteur Gen III standard ; actuellement décodé de façon conservative avec 1 paramètre, ce qui peut désaligner localement une chaîne si un sous-code en prend 2. À affiner si des désalignements sont observés lors de l'extraction exhaustive.
| `0xFD xx` | insertion de variable (nom du joueur, buffer de string de script, etc. — ex. `<FD:02>` = « hatched from the EGG! », `<FD:04>` = nom du Pokémon donné à couver) |

Exemples réels extraits (preuve) :
```
'Communication error…<FE>Please check all connections,<FE>then turn the power OFF and ON.'
'Would you like to save the game?'
'<FD:02> hatched from the EGG!'
'What should<FE><FD:12> do?'
```

## 5. Espace libre pour réinsertion

`tools/find_free_space.py` : **15,93 MiB** de runs contigus `0xFF` ≥1024 octets (≥1 Ko), essentiellement dans la zone d'extension `0x00F00000`–`0x02000000` ajoutée par le hack par rapport à la FireRed d'origine (16 MiB). Plus gros blocs :

- `0x1413A9C`–`0x1A00000` (6,21 Mo)
- `0x1B227B0`–`0x1EB0B30` (3,73 Mo)
- `0x0930280`–`0x0A00000` (0,85 Mo)

→ Largement suffisant pour reloger toutes les chaînes françaises plus longues que l'original, avec marge très confortable.

## 6. Compression

Le format LZ77 standard BIOS GBA (octet `0x10` + taille 3 octets LE) est bien présent et décompresse proprement (vérifié sur des dizaines de blocs graphiques). **Le texte de dialogue lui-même n'est pas compressé** (les chaînes trouvées via `scan_pointers.py` sont directement lisibles sans décompression), ce qui simplifie beaucoup la réinsertion : pas de recompression LZ77 nécessaire pour le texte.

## 7. Limite des boîtes de dialogue

Non mesurée précisément pour l'instant (nécessite un test d'affichage réel — cf. section 3). Par convention FireRed standard, une boîte de dialogue affiche typiquement ~2 lignes de ~36-38 caractères avant `<FE>`/`<FB>` — **à vérifier empiriquement lot par lot** plutôt que supposé comme règle absolue, conformément à la consigne du projet.

## 8. Stratégie d'expansion sûre retenue

Conforme à la règle n°10 du mandat :
1. Traduction plus courte ou égale à l'original → réinsertion en place, aucun changement de pointeur.
2. Traduction plus longue → écriture dans l'espace libre identifié section 5, mise à jour du pointeur d'origine, entrée journalisée dans `build/patch_log.tsv` (ancien pointeur → nouveau, ancienne cible → nouvelle, taille).

## 9. Ce qui reste à faire avant "terminé"

- Vérification visuelle réelle des caractères accentués français au-delà de `é` (nécessite émulateur avec affichage — non disponible dans cette session).
- Extraction exhaustive (au-delà des 5866 pointeurs de ce premier passage, qui ne couvrent qu'une partie de la ROM — le scan complet croisé avec les tables de scripts reste à faire, voir `docs/PROGRESS.md`).
- Détermination précise du nombre de paramètres de chaque sous-code `0xFC xx`.
- Mesure réelle de la limite de largeur des boîtes de dialogue.
