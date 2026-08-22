# Suivi de progression — traduction française Pokémon Odyssey

Dernière mise à jour : 2026-08-22 (en cours, traduction complète en cours d'exécution lot par lot, sur autorisation explicite de l'utilisateur : « Tout bon tu peux traduire l'entièreté du jeu »)

## État réel (pas une estimation arrondie)

| Métrique | Valeur |
|---|---|
| Chaînes extraites (texte pointé, base) | **9 051** |
| … dont bruit de décodage (fragments non exploitables, jamais supprimées) | 2 332 (`SKIP_NOISE`) |
| Chaînes de texte réel à traiter (9051 - 2332) | 6 719 |
| Traduites (`TRANSLATED`) | **4 050** |
| Restant à traduire | 2 669 (`UNTRANSLATED`) |
| Validées structurellement (contrôle des codes, encodage) | 4050 / 4050, 0 erreur |
| Réinsérées dans un build ROM | 4050 / 4050 (`build/Pokemon_Odyssey_FR.gba`) |
| Testées en jeu (affichage réel vérifié) | Caractères accentués français confirmés par l'utilisateur sur émulateur réel (voir `TESTING.md`). Le reste du contenu traduit n'a pas été rejoué en jeu depuis (volume trop important pour un test exhaustif manuel à ce stade).

**Pourcentage de traduction du texte réel : ~60 % (4050 / 6719 chaînes réelles utiles).** Ne pas arrondir vers le haut.

Contenu couvert à ce stade (non exhaustif) : intégralité du LOT 1 (interface/système), Mt. Moon, S.S. Anne, Team Rocket (Mt. Moon/Silph Co./Warehouse/Hideout), Victory Road, Pokémon Tower, Safari Zone, la plupart des Arènes Kanto, Îles Sevii (One-Seven Island, Bill/Celio, ruines Tanoby), système d'AIDE complet, Game Corner, panneaux de lieux, PNJ dresseurs de plusieurs routes, crédits du hack, interface Mystery Gift/Wonder Card/Wireless Communication System complète, chat sans fil, une large partie du Pokédex (Bulbasaur → Gengar/Onix, Cacnea → Relicanth, avec plusieurs entrées de lore propres au hack comme le "Deep One"/"Abyssal God"), mobilier complet de Secret Base (bureaux, chaises, tapis, posters, peluches, coussins), noms de Nature (10/25 rencontrés jusqu'ici), et un grand bloc de descriptions de capacités de combat (~300 capacités).

## Ce qui est fait et vérifié

- Audit technique complet (`docs/TECHNICAL_AUDIT.md`) : ROM identifiée (hack de FireRed BPRE, 32 Mio), pointeurs GBA standards confirmés empiriquement sur plusieurs milliers d'occurrences, table de caractères confirmée pour la ponctuation/chiffres/A-Z/a-z/é ET pour les accents français étendus (à â ç è ê ë î ï ô œ ù û + majuscules), confirmé par test en émulateur réel. Terminateur `0xFF`, codes de contrôle `<FE> <FA> <FB> <FC:xx> <FD:xx>` identifiés avec exemples réels.
- 15,93 Mio d'espace libre repéré pour la relocalisation des chaînes plus longues (`tools/find_free_space.py`).
- Risque de pointeurs qui partagent les mêmes octets physiques (technique d'économie d'espace) détecté et neutralisé : 906 lignes forcées en relocalisation systématique pour éviter toute corruption croisée.
- Pipeline complet et fonctionnel, de bout en bout : extraction fusion-safe (`tools/extract_text.py`) → traduction par lots (`translation/text_database.tsv`, outils `tools/batch_dump.py` / `tools/batch_apply.py`) → validation (`tools/validate_text.py`) → réinsertion (`tools/build_french_rom.py`) → validation ROM (`tools/validate_rom.py`).
- Glossaire de cohérence terminologique (`translation/glossary.tsv`) : noms de lieux/personnages/Pokémon/capacités/Natures officiels, avec REVIEW explicite sur toute incertitude (quelques noms d'espèces et de capacités non garantis à 100 % faute de vérification externe).
- Build ROM courant : 180 050 octets modifiés sur 33 554 432 (0,54 %), taille/header/checksum GBA intacts.

## Ce qui N'EST PAS fait (à ne pas prétendre terminé)

- **2 669 chaînes de texte réel restantes** : essentiellement dialogues de PNJ non couverts, fin du Pokédex national, objets/descriptions de boutique restants, et du contenu tardif/postgame spécifique au hack (Sevii Islands étendu, éventuel contenu Yggdrasil-Labyrinth) pas encore localisé avec certitude.
- **Aucun nouveau test visuel en jeu depuis la confirmation initiale des accents.** Le volume traduit a été multiplié par plus de 60 depuis ce test ; aucune vérification humaine sur émulateur n'a eu lieu sur le contenu ajouté depuis (mise en page des boîtes de dialogue, débordement de texte, alignement des tableaux d'objets/capacités).
- Largeur exacte des boîtes de dialogue non mesurée automatiquement (le validateur ne fait qu'avertir au-delà de 2x la longueur anglaise, pas de contrainte dure).
- Quelques noms d'espèces/capacités marqués REVIEW dans le glossaire faute de certitude absolue sur la graphie officielle exacte (ex. Treecko, Swablu, Relicanth, Spinda, Wingull, Gulpin).
- `translation/context_database.tsv` (carte/événement/flags par chaîne) non créé — contextualisation uniquement via la colonne `context` en texte libre, pas de mapping systématique scène↔chaîne.

## Prochaines étapes recommandées

1. Continuer la traduction lot par lot (200 lignes/lot environ) jusqu'à couverture complète des 2 669 chaînes restantes, en revalidant et reconstruisant à chaque lot (déjà en cours, automatisé).
2. Une fois le texte réel complet, faire un nouveau test en jeu réel (émulateur avec affichage) sur un échantillon représentatif couvrant plusieurs zones du jeu, pas seulement l'écran de sauvegarde.
3. Repasser sur les entrées REVIEW du glossaire pour confirmer ou corriger les noms officiels incertains.
4. Livrable final : `build/Pokemon_Odyssey_FR.gba` + SHA-256, uniquement une fois le pourcentage réel proche de 100 % et honnêtement documenté ici.
