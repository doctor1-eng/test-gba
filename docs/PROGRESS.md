# Suivi de progression — traduction française Pokémon Odyssey

Dernière mise à jour : 2026-08-22 (traduction complète de tout le texte réel identifié, sur autorisation explicite de l'utilisateur : « Tout bon tu peux traduire l'entièreté du jeu »)

## État réel (pas une estimation arrondie)

| Métrique | Valeur |
|---|---|
| Chaînes extraites (texte pointé, base) | **9 051** |
| … dont bruit de décodage / données non textuelles (jamais supprimées, marquées `SKIP_NOISE`) | **2 717** |
| Chaînes de texte réel (9051 - 2717) | **6 334** |
| Traduites (`TRANSLATED`) | **6 334 / 6 334 → 100 %** |
| Validées structurellement (contrôle des codes, encodage) | 6 334 / 6 334, **0 erreur** |
| Réinsérées dans le build ROM | 6 334 / 6 334 (`build/Pokemon_Odyssey_FR.gba`) |
| Testées en jeu (affichage réel vérifié) | Caractères accentués français confirmés par l'utilisateur sur émulateur réel au tout début du projet (voir `TESTING.md`). **Le contenu ajouté depuis n'a pas été retesté visuellement en jeu** — voir limitations ci-dessous. |

**100 % du texte réel traduit.** Ce chiffre est honnête au sens strict : il couvre toutes les lignes classées comme texte réel exploitable par l'extracteur. Il ne garantit pas que 100 % du texte soit *parfait* — voir les limitations ci-dessous.

## Composition des 2 717 lignes `SKIP_NOISE` (bruit, non traduites intentionnellement)

Toutes conservées, jamais supprimées, chacune avec une note explicite dans la colonne `context` :
- **Fragments de chaînes qui se chevauchent en mémoire** (technique d'économie d'espace du ROM d'origine) dont le texte utile a déjà été traduit via la ligne parente — reclassés seulement quand la traduction du fragment était authentiquement injouable sans inventer du contenu.
- **~330 lignes de bruit de décodage pur** : séquences de symboles répétés, données binaires/graphiques mal identifiées comme texte par l'extracteur une fois la table de caractères complétée avec les accents français (le bruit était auparavant filtré différemment ; il a été détecté et isolé méthodiquement lors de cette session, avec vérification manuelle par échantillonnage avant toute reclassification en masse).
- **Une quinzaine de lignes en italien** : ce hack contient, de façon inattendue, des fragments de texte italien resté dans les tables du jeu (probablement un vestige d'une localisation italienne antérieure ou parallèle). Ce texte n'est pas traduit vers le français — il appartient à une autre langue du jeu et le traduire aurait été une erreur de portée, pas une traduction légitime.
- **7 fragments anglais authentiques mais trop courts/ambigus** pour être traduits sans inventer du contenu (aucune chaîne parente récupérable dans le contexte disponible).

## Ce qui est fait et vérifié

- Audit technique complet (`docs/TECHNICAL_AUDIT.md`) : ROM identifiée (hack de FireRed BPRE, 32 Mio), pointeurs GBA standards confirmés empiriquement sur plusieurs milliers d'occurrences, table de caractères confirmée pour la ponctuation/chiffres/A-Z/a-z/é ET pour les accents français étendus (à â ç è ê ë î ï ô œ ù û + majuscules), confirmé par test en émulateur réel. Terminateur `0xFF`, codes de contrôle `<FE> <FA> <FB> <FC:xx> <FD:xx>` identifiés avec exemples réels.
- 15,93 Mio d'espace libre repéré pour la relocalisation des chaînes plus longues (`tools/find_free_space.py`).
- Risque de pointeurs qui partagent les mêmes octets physiques (technique d'économie d'espace) détecté et neutralisé : 906 lignes forcées en relocalisation systématique pour éviter toute corruption croisée.
- Pipeline complet et fonctionnel, de bout en bout : extraction fusion-safe (`tools/extract_text.py`) → traduction par lots (`translation/text_database.tsv`, outils `tools/batch_dump.py` / `tools/batch_apply.py`) → validation (`tools/validate_text.py`) → réinsertion (`tools/build_french_rom.py`) → validation ROM (`tools/validate_rom.py`).
- Glossaire de cohérence terminologique (`translation/glossary.tsv`, ~250 entrées) : noms de lieux/personnages/Pokémon/capacités/Natures officiels, avec REVIEW explicite sur toute incertitude (36 lignes du texte traduit portent une note REVIEW dans leur contexte).
- Build ROM final : 340 886 octets modifiés sur 33 554 432 (1,02 %), taille/header/checksum GBA intacts, SHA-256 documenté ci-dessous.
- Couverture : intégralité de l'histoire principale connue (intro archaïque, scénario Kanto classique, scénario du hack — Yggdrasil/Abyssaux/labyrinthe à strates, fin du jeu et épilogue), tout le système d'aide et de tutoriel standard, tous les objets/CT/capacités/talents rencontrés dans l'extraction, tout le Pokédex national accessible (espèces classiques + créatures F.O.E./légendaires propres au hack), toutes les quêtes annexes identifiées, tout le mobilier de Secret Base, tout le système Mystery Gift/Wireless/Trading, les crédits du hack.

## Ce qui N'EST PAS fait / limitations honnêtes (à ne pas prétendre parfait)

- **Pas de nouveau test visuel en jeu depuis la confirmation initiale des accents.** Le volume traduit a été multiplié par plus de 100 depuis ce test unique sur l'écran de sauvegarde. Aucune vérification humaine sur émulateur n'a eu lieu sur la mise en page réelle des boîtes de dialogue, le débordement de texte, ou le rendu des menus/tableaux d'objets. **C'est la limitation la plus importante restante.**
- **36 termes officiels non garantis à 100 %** (noms de talents, quelques noms de Pokémon/capacités rares, quelques attributions Chef d'Arène) — traductions descriptives raisonnables faites de mémoire, marquées REVIEW dans le glossaire, à vérifier contre une base de données Pokémon FR officielle avant une éventuelle diffusion publique.
- Largeur exacte des boîtes de dialogue non mesurée automatiquement (le validateur ne fait qu'avertir au-delà de 2x la longueur anglaise, pas de contrainte dure) — le français est structurellement ~15-20 % plus long que l'anglais, un débordement visuel ponctuel est possible sans test en jeu.
- Guillemets français (« ») non disponibles dans la table de caractères confirmée — les guillemets typographiques anglais (" ") ont été utilisés systématiquement à la place, ce qui est un compromis technique assumé, pas une erreur.
- `translation/context_database.tsv` (carte/événement/flags par chaîne) non créé — la contextualisation reste au niveau de la colonne `context` en texte libre par ligne.
- Les ~15 lignes italiennes et les fragments trop courts non traduits restent dans le fichier avec leur texte d'origine, correctement documentés, jamais supprimés.

## Livrables

- `build/Pokemon_Odyssey_FR.gba` — ROM française finale (non versionnée dans git par politique de copyright, voir `README.md` ; livrée directement à l'utilisateur).
- `build/Pokemon_Odyssey_FR.sha256` — empreinte SHA-256 du build.
- `build/patch_log.tsv` — journal complet ancien→nouveau pointeur pour chaque relocalisation.
- `translation/text_database.tsv` — base de données de traduction complète (9 051 lignes), versionnée.
- `translation/glossary.tsv` — glossaire terminologique, versionné.

## Prochaines étapes recommandées (si le projet continue au-delà de cette session)

1. **Test en jeu réel** sur un émulateur avec affichage, en couvrant plusieurs zones représentatives (intro, une ville Kanto, un donjon du hack, un combat, un menu d'objets) — c'est l'étape manquante la plus importante avant toute diffusion.
2. Repasser sur les 36 entrées REVIEW du glossaire pour confirmer ou corriger les noms officiels incertains contre une base de données Pokémon FR fiable.
3. Si souhaité, ré-examiner les ~15 lignes italiennes pour décider si elles doivent être traduites en français (actuellement laissées dans leur langue d'origine, hors du périmètre de la mission qui demandait une traduction anglais→français) ou si elles sont réellement inutilisées dans le jeu.
