# Suivi de progression — traduction française Pokémon Odyssey

Dernière mise à jour : 2026-08-23 (quatrième correctif de corruption ROM, cette fois confirmé visuellement via un vrai émulateur headless — voir « Quatrième bug » ci-dessous)

## État réel (pas une estimation arrondie)

| Métrique | Valeur |
|---|---|
| Chaînes extraites (texte pointé, base) | **9 051** |
| … dont bruit de décodage / données non textuelles (jamais supprimées, marquées `SKIP_NOISE`) | **2 717** |
| Chaînes de texte réel (9051 - 2717) | **6 334** |
| Traduites (`TRANSLATED`) | **6 334 / 6 334 → 100 %** |
| Validées structurellement (contrôle des codes, encodage) | 6 334 / 6 334, **0 erreur** |
| Réinsérées dans le build ROM | 6 298 / 6 334 (`build/Pokemon_Odyssey_FR.gba`) — 36 lignes volontairement laissées en anglais par sécurité anti-corruption, voir « Quatrième bug » ci-dessous |
| Testées en jeu (affichage réel vérifié) | Caractères accentués français confirmés par l'utilisateur sur émulateur réel au tout début du projet (voir `TESTING.md`). Deux bugs de corruption signalés par l'utilisateur en jeu réel (vidéos) ont été corrigés. **Un test headless via un vrai émulateur (mGBA en liaison Python, 2026-08-23) confirme désormais visuellement l'écran-titre et l'intégralité de la narration d'introduction** — la première confirmation par image réelle plutôt que par raisonnement seul dans ce projet. Le reste du jeu (menus, combats, zones avancées) n'a pas été parcouru exhaustivement. |

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

**Ce premier correctif s'est révélé insuffisant** : l'utilisateur a retesté le build corrigé (SHA-256 `be77f2ee...`) et a rapporté exactement les mêmes symptômes, avec la demande explicite de vérifier l'intégrité de l'ensemble des fichiers.

## Second bug, beaucoup plus large, trouvé sur re-vérification complète (2026-08-23)

Suite au signalement « toujours pas », un audit complet a été relancé plutôt que de supposer le premier correctif suffisant (conformément à la règle du mandat : ne jamais traiter une extraction heuristique comme une preuve de structure). Plusieurs pistes ont été vérifiées et éliminées avec preuves à l'appui avant de trouver la vraie cause :
- Allocation d'espace libre : vérifiée saine (un seul bloc de 5,92 Mio, largement contigu, seulement 284 Ko réellement utilisés — aucun risque de recouvrement avec des données utilisées).
- Détection des chevauchements de pointeurs : re-vérifiée par un algorithme exhaustif (pas seulement les paires adjacentes) — 0 ligne manquée.
- Adressage au-delà de 16 Mio (ROM étendue à 32 Mio) : écarté avec preuve directe — des messages de combat *vanilla* absolument essentiels (« X gained EXP. Points! », « X fainted! ») vivent déjà au-delà de 16 Mio dans le ROM anglais d'origine et s'affichent normalement, donc le moteur du jeu gère bien ces adresses nativement.
- Comptage d'octets des codes de contrôle `<F8>`/`<F9>`/`<FC:xx>` : anomalie statistique réelle détectée (le octet suivant ces codes tombe dans la plage `0x00`-`0x28` bien plus souvent que la normale), mais sans impact concret car chaque caractère « mystère » avait déjà été préservé tel quel par la traduction (technique de préservation de placeholder déjà en usage) — inoffensif en pratique, documenté pour référence future.

**La vraie cause : deux caractères accentués jamais réellement vérifiés à l'écran, mais déclarés "confirmés" par erreur.** Le test de jeu de caractères du 2026-08-22 (`tools/build_charset_test.py`, résultat dans `TESTING.md`) affichait la chaîne `"à â ç è ê ë î ï ô ù û"` / `"À Â Ç È Ê Ë Î Ï Ô Ù Û Œ œ"` sur un émulateur réel, confirmée visuellement par l'utilisateur. **Mais cette chaîne de test ne contient ni `é` ni `É`** — ces deux lettres ont été oubliées du texte affiché à l'écran. Or la mise à jour "CONFIRMÉ" de `docs/TECHNICAL_AUDIT.md` a promu **l'intégralité** de la plage basse (`0x01`-`0x28`) comme validée, y compris `0x06='É'` et `0x1B='é'`, qui n'avaient donc jamais été réellement vus à l'écran.

Pire : `0x1B='é'` était un **doublon silencieux** d'un octet différent, `0xF7`, qui lui était authentiquement confirmé (présent dans le mot « Pokémon » lisible dans le ROM). Comme `0x1B` était défini plus tard dans le dictionnaire Python `CHARMAP`, c'est lui qui gagnait lors de la construction de la table inverse utilisée pour encoder le français — **tout `é` tapé par un traducteur dans ce projet a donc systématiquement été écrit dans le ROM avec l'octet jamais vérifié `0x1B`, jamais avec le `0xF7` réellement confirmé.**

Impact mesuré : **2 879 lignes sur 6 334 traduites (45,5 %) contiennent la lettre `é`** — largement la lettre accentuée la plus fréquente du français (verbes au participe passé, « été », « café », etc.). Si `0x1B` ne s'affiche pas correctement dans cette ROM précise (ce qui est resté non vérifié jusqu'ici), près de la moitié de tout le texte traduit afficherait un caractère faux, un tuile vide, ou un artefact graphique à chaque `é` — une explication beaucoup plus cohérente avec « les dialogues ne s'affichent pas » que les 2 lignes touchées par le premier bug.

**Correctif appliqué** dans `tools/gen3_charmap.py` :
- Retrait de `0x1B: 'é'` de `CHARMAP` → la table inverse retombe automatiquement sur le seul octet réellement confirmé, `0xF7`.
- Retrait de `0x06: 'É'` (aucune alternative confirmée n'existe) → repli automatique sur `E` sans accent via `ASCII_FALLBACK`, une convention typographique française classique et sans risque (365 lignes / 5,8 % concernées, chacune marquée `REVIEW` par `tools/validate_text.py`, 0 erreur bloquante).

Nouveau build : 339 039 octets modifiés (1,010 %). `tools/validate_rom.py` confirme taille/en-tête/checksum intacts. Vérifié : l'usage de l'octet `0xF7` dans le ROM final augmente de 5 775 occurrences par rapport à l'original, cohérent avec la redirection de tous les `é` nouvellement traduits.

**Nouveau SHA-256 : voir `build/Pokemon_Odyssey_FR.sha256`** (remplace `be77f2ee...`).

**Limitation honnête** : sans émulateur avec affichage disponible dans cet environnement (tenté via Xvfb + mGBA headless — même le ROM anglais d'origine, non modifié, restait à l'écran noir dans ce montage, donc le test n'apporte aucune preuve dans un sens ou l'autre), ce correctif n'a **pas pu être re-confirmé visuellement** avant livraison. Il repose sur un raisonnement de traçabilité rigoureux (le test réel du 2026-08-22 n'a jamais montré `é`/`É` à l'écran, malgré la mention "confirmé") plutôt que sur une nouvelle preuve visuelle. **Seul un nouveau test par l'utilisateur confirmera si ce second correctif résout réellement le problème.**

## Troisième bug : le premier correctif de références n'était pas assez strict (2026-08-23)

L'utilisateur a fourni une nouvelle vidéo de test du build corrigé (SHA-256 `f81570ad...`). Analyse image par image (`ffmpeg` + inspection visuelle) :
- L'intro cinématique **s'affiche désormais correctement** (« YGGDRASIL PROJECT » avec portrait de personnage) — ce point est bien résolu.
- Juste après, l'écran enchaîne une bande de bruit statique → noir plein → deux cadres de boîte de dialogue vides (silhouette de personnage en fond, **mais aucun texte ne s'affiche jamais dedans**) → marron plein → noir plein à nouveau, sans jamais se rétablir malgré des appuis répétés. C'est exactement le symptôme original (« fond noir qui change de couleur aléatoirement », « dialogues ne s'affichent pas »), toujours présent.
- Le dernier "écran" de la vidéo n'est pas le jeu : c'est le Centre de contrôle iOS affichant le texte système « À l'arrêt » (statut du lecteur média) — sans rapport avec le jeu, probablement une confusion de l'utilisateur en interprétant ce texte système comme un bouton.

Cette séquence corrompue correspond exactement à l'emplacement de la narration d'introduction (« ...draw countless explorers from all over the world... ») déjà identifiée comme zone à risque. En creusant plus loin dans la même famille de fragments qui se chevauchent (`006135`-`006152`), **4 lignes de plus** (`006145`, `006146`, `006149`, `006150`) avaient des listes de références ambiguës (2 à 27 références brutes, dispersées sur plusieurs Mio, sans cluster dominant) que le garde-fou du premier correctif (seuil de rétention à 60 % sur les listes de plus de 40) ne couvrait pas — ces listes plus courtes passaient sous le radar du seuil.

**Correctif affiné** dans `tools/build_french_rom.py` (`trustworthy_refs()`) : un second mode « strict » s'applique désormais spécifiquement aux lignes qui partagent physiquement des octets avec une voisine (`overlapping_ids`, la même famille que la narration d'intro) — celles-ci n'ont le droit à une relocalisation que si le cluster de références est **unanime** (100 % d'accord) ou représente un motif de table clairement écrasant (≥ 20 références, ≥ 90 % retenues). Les lignes standalone (non chevauchantes) gardent la règle d'origine, plus permissive : vérifié explicitement que ~230 lignes de description de talent ont chacune exactement 2 références légitimes, à des endroits différents du ROM (probablement écran Résumé + liste des talents), et qu'aucune n'appartient à une famille de chevauchement — leur appliquer la règle stricte les aurait faussement fait échouer.

Résultat : 34 lignes sur 6 334 sont désormais laissées en anglais par sécurité (contre 2 après le premier correctif), toutes vérifiées comme faisant partie de familles de chevauchement physique. Nouveau build : 336 376 octets modifiés (1,002 %). `tools/validate_rom.py` confirme taille/en-tête/checksum intacts.

**Nouveau SHA-256 : voir `build/Pokemon_Odyssey_FR.sha256`** (remplace `f81570ad...`).

**Limitation honnête** : comme pour le correctif précédent, aucune confirmation visuelle n'a pu être faite dans cet environnement avant livraison. Ce correctif est ciblé précisément sur la zone montrée dans la vidéo de l'utilisateur (narration d'intro), mais il reste possible que d'autres zones du jeu (non visitées dans les vidéos de test jusqu'ici) contiennent des problèmes similaires non encore détectés.

## Quatrième bug, trouvé ET confirmé visuellement pour la première fois via un vrai émulateur headless (2026-08-23)

L'utilisateur a demandé de tester directement le build livré. Plutôt que de refaire une analyse statique, une vraie capacité de test visuel a été mise en place dans cette session : les liaisons Python officielles de mGBA (`pip install mgba`) pilotent le cœur d'émulation en mémoire, sans SDL ni serveur d'affichage — contrairement à `mgba-sdl` sous Xvfb (essayé lors des corrections précédentes), qui ne rendait jamais rien dans cet environnement, y compris pour la ROM anglaise d'origine intacte. Voir `TESTING.md` pour les outils (`tools/headless_playtest.py`, `tools/replay_patch_log.py`) et leur usage.

**Premier test réel** : capture d'écran automatisée de l'écran-titre (identique séquence de touches pour la ROM EN et FR). Résultat : **le logo « Pokémon Odyssey » était toujours remplacé par un motif de losanges corrompu répétitif** dans le build FR le plus récent — un tout autre bug que les trois précédents, présent dès le tout premier écran du jeu.

**Cause trouvée par bissection automatique** (et non plus par déduction manuelle) : `tools/replay_patch_log.py` a permis de reconstruire la ROM en ne rejouant qu'un préfixe de `build/patch_log.tsv`, combiné à une recherche dichotomique automatisée sur `tools/headless_playtest.py` — 12 itérations ont isolé la ligne fautive : `ODYSSEY-TXT-004785` (description de capacité « Drain Punch »), qui avait **exactement une seule référence brute**, à l'adresse `0x0000D90`. Vérification directe des octets du ROM d'origine à cette adresse : code machine ARM/THUMB dense et à haute entropie (le tout début du code de démarrage/gestionnaires d'interruption du ROM, bien avant que la moindre table de pointeurs de texte ne puisse exister — l'en-tête de cartouche GBA fait `0xC0` octets et le code de boot occupe encore plusieurs Ko après). Ce n'était clairement pas un pointeur de texte légitime.

**La faille précise** : les deux correctifs précédents (`trustworthy_refs()`) ne s'appliquaient qu'à partir de 2 références (clustering, seuils stricts/permissifs) — mais une ligne avec **une seule référence** passait toujours sans aucune validation (`if len(refs) <= 1: return refs`), sur l'hypothèse que la médiane de la base (1 référence réelle par ligne) rendait ce cas presque toujours sûr. Cette instance prouve que même une référence unique peut être une coïncidence.

**Correctif** : ajout d'un plancher d'adresse plausible (`MIN_PLAUSIBLE_REF_ADDR = 0x8000`, soit 32 Kio) appliqué à **toute** ligne avant même de compter les références — toute adresse en dessous est rejetée d'office, qu'il y en ait une ou cent. Sur toute la base de données, seulement 14 références sur 11 015 tombent sous ce seuil (dont 8 dans des lignes `SKIP_NOISE` jamais insérées) ; seules 2 lignes `TRANSLATED` étaient concernées (`004785` et `004376`, « SMALL DESK »/petit bureau, référence à `0x14C`, en plein dans la zone d'en-tête/vecteurs d'entrée).

**Vérification** : après correctif, l'écran-titre a été recapturé — **logo « Pokémon Odyssey » correctement affiché**, variance de pixels (`std`) quasi identique à l'original (64,5/64,0/53,3 contre 64,3/63,7/53,6 pour l'anglais). Test étendu à 150 points de contrôle en mitraillant le bouton A à travers toute l'introduction et le début du jeu : les courbes de variance EN/FR se suivent de très près sur l'ensemble de la séquence (contre un effondrement vers une couleur unie observé avant correctif). Capture d'écran de la narration d'intro également confirmée : fond graphique correct (arbre/racines/coffre) avec texte français lisible par-dessus. **C'est la première fois dans ce projet qu'un correctif est confirmé par une image réelle plutôt que par un raisonnement seul.**

Nouveau build : 36 lignes laissées en anglais par sécurité (contre 34), 336 298 octets modifiés (1,002 %). `tools/validate_rom.py` confirme taille/en-tête/checksum intacts. **Nouveau SHA-256 : voir `build/Pokemon_Odyssey_FR.sha256`** (remplace `9fa0f77c...`).

**Découverte annexe, non corrigée (hors périmètre)** : le test approfondi a aussi révélé au moins 2 fragments de la narration d'intro qui restent en anglais parce qu'ils sont référencés par une adresse **calculée/indirecte** (aucun pointeur absolu, aligné ou non, ne pointe vers eux nulle part dans le ROM), invisible pour la méthode de scan actuelle. Ce n'est pas une corruption — juste une lacune de couverture déjà anticipée dans `docs/TECHNICAL_AUDIT.md` section 9, maintenant confirmée avec un exemple concret. Non corrigé dans cette session (nécessiterait de rétro-ingénierer le mécanisme d'adressage du moteur de script).

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
