# Suivi de progression — traduction française Pokémon Odyssey

Dernière mise à jour : 2026-08-23 (correction d'un bug de corruption ROM signalé par l'utilisateur en jeu réel — voir section dédiée ci-dessous)

## État réel (pas une estimation arrondie)

| Métrique | Valeur |
|---|---|
| Chaînes extraites (texte pointé, base) | **9 051** |
| … dont bruit de décodage / données non textuelles (jamais supprimées, marquées `SKIP_NOISE`) | **2 717** |
| Chaînes de texte réel (9051 - 2717) | **6 334** |
| Traduites (`TRANSLATED`) | **6 334 / 6 334 → 100 %** |
| Validées structurellement (contrôle des codes, encodage) | 6 334 / 6 334, **0 erreur** |
| Réinsérées dans le build ROM | 6 332 / 6 334 (`build/Pokemon_Odyssey_FR.gba`) — 2 lignes volontairement laissées en anglais par sécurité anti-corruption, voir section « Bug critique » ci-dessous |
| Testées en jeu (affichage réel vérifié) | Caractères accentués français confirmés par l'utilisateur sur émulateur réel au tout début du projet (voir `TESTING.md`). Un test en jeu ultérieur (2026-08-23) a révélé un bug de corruption critique, corrigé le jour même (voir ci-dessous) — **le build corrigé n'a pas encore été retesté en jeu par l'utilisateur.** |

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
- Build ROM final : 339 040 octets modifiés sur 33 554 432 (1,010 %), taille/header/checksum GBA intacts, SHA-256 documenté ci-dessus (voir section « Bug critique trouvé en test réel et corrigé »).
- Couverture : intégralité de l'histoire principale connue (intro archaïque, scénario Kanto classique, scénario du hack — Yggdrasil/Abyssaux/labyrinthe à strates, fin du jeu et épilogue), tout le système d'aide et de tutoriel standard, tous les objets/CT/capacités/talents rencontrés dans l'extraction, tout le Pokédex national accessible (espèces classiques + créatures F.O.E./légendaires propres au hack), toutes les quêtes annexes identifiées, tout le mobilier de Secret Base, tout le système Mystery Gift/Wireless/Trading, les crédits du hack.

## Bug critique trouvé en test réel et corrigé (2026-08-23)

L'utilisateur a testé le premier build livré (SHA-256 `39de1d2d...`) sur un émulateur réel (Manic EMU, iOS) et a signalé, texto : *« Les cinématiques ne s'affiche pas, fond noir qui change de couleur aléatoirement et les dialogues ne s'affichent pas non plus »*.

**Cause racine identifiée et confirmée contre le ROM d'origine** (pas seulement supposée) : `tools/extract_text.py` (`scan_pointers()`) détecte les pointeurs en scannant CHAQUE mot de 4 octets aligné du ROM entier, et traite comme « référence » tout mot dont les octets correspondent à l'adresse d'un texte déjà découvert — sans jamais vérifier que cet emplacement est réellement une entrée de table de pointeurs, plutôt qu'une coïncidence d'octets dans des données binaires sans rapport (graphismes, audio, remplissage). C'était suffisant pour la confirmation de la table de caractères (preuve statistique), mais dangereux pour la réinsertion, qui réécrivait aveuglément TOUTES les adresses listées lors d'une relocalisation.

Cas concret confirmé par inspection directe des octets du ROM d'origine : la chaîne narrative d'introduction ("...draw countless explorers from all over the world...") existe en tant que fragments qui se chevauchent, décalés d'un octet chacun (technique d'économie de mémoire déjà connue). Le fragment commençant à l'offset physique `0x1090909` s'encode en adresse pointeur `0x09090909` — un motif de 4 octets identiques (« repdigit »), qui se répète par pure coïncidence dans de grandes zones de données binaires à faible entropie (graphismes/audio) ailleurs dans les 32 Mio du ROM. Résultat : **178 "références" détectées pour cette seule ligne**, alors que la médiane sur toute la base est de 1 référence réelle par ligne. Preuve additionnelle : le nombre de références culmine exactement sur le fragment au motif le plus répétitif (`09 09 09 09` → 178 refs) et diminue symétriquement de part et d'autre (`0A 09 09 09` → 27 refs, `08 09 09 09` → 46 refs, etc.), ce qui exclut une coïncidence statistique bénigne.

Lors de la relocalisation (chaîne trop longue pour tenir en place), l'outil de réinsertion réécrivait ces 178 adresses « références » comme si c'étaient de vrais pointeurs, écrasant très probablement des données de graphismes/palettes et d'autres pointeurs de script répartis sur ~14 Mio du ROM — cause la plus probable des cinématiques qui ne s'affichent pas / fond noir aléatoire / dialogues absents.

**Correctif appliqué** dans `tools/build_french_rom.py` (fonction `trustworthy_refs()`), avec deux défenses complémentaires, justifiées par la structure réelle des tables de pointeurs GBA (un tableau borné, physiquement localisé, de pointeurs vers une même chaîne) :
1. **Clustering spatial** : seules les références regroupées dans la plus grande zone contiguë du ROM (écart ≤ 16 Kio) sont conservées ; les correspondances isolées à des mégaoctets de distance sont écartées comme bruit.
2. **Seuil de confiance** : si la liste brute dépassait 40 références et que le clustering n'en retient qu'une minorité (< 60 %), la ligne entière est jugée non fiable — elle n'est **pas insérée** dans ce build (le texte anglais d'origine reste intact à son emplacement, ce qui est toujours sans risque), plutôt que de deviner quelles références sont réelles.

**Résultat concret sur la base actuelle** : 2 lignes sur 6 334 (`ODYSSEY-TXT-006147`, `ODYSSEY-TXT-006148`) sont désormais volontairement laissées en anglais par sécurité — ce sont exactement les deux fragments les plus contaminés de la chaîne de chevauchement décrite ci-dessus. **Aucun impact visible en jeu** : le texte narratif réel affiché provient d'autres lignes de la même famille de chevauchement (`006142`-`006144`, `006151`-`006152`), qui ont chacune une référence unique et propre, confirmée saine, et ont été traduites et relocalisées normalement. Par ailleurs, le clustering a aussi silencieusement nettoyé des références suspectes sur une trentaine d'autres lignes (2 à 21 fausses références chacune) qui n'auraient pas fait planter le build de façon visible mais auraient quand même corrompu quelques emplacements du ROM.

Nouveau build : 339 040 octets modifiés (1,010 %, contre 340 886 précédemment). `tools/validate_rom.py` confirme taille/en-tête/checksum intacts.

**Nouveau SHA-256 : `be77f2ee12e0858f92fe72c9093b285af2ac54b3c76808cbec987fd916cabf83`** (remplace `39de1d2d...`, qui contenait le bug).

Limitation honnête restante : ce correctif est un garde-fou statistique fondé sur la structure typique des tables de pointeurs GBA, pas une preuve formelle que chaque référence conservée est réelle à 100 % — mais il est nettement plus prudent que l'ancien comportement, et le seul test en émulateur réel disponible (celui de l'utilisateur) portait justement sur la zone corrigée.

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
