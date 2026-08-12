# Rapport final — Analyse statique Pokémon Unbound v2.1.1.1

Analyse d'un pack de métadonnées statistiques (11 CSV/JSON/MD, 34 Mo décompressés), pas de la ROM
elle-même. Voir `README.md` pour le contexte complet et les quatre refus d'analyse de fichiers ROM qui
ont précédé cette étape.

## 1. Qu'avons-nous réellement réussi à déterminer ?

- L'identité de base d'Unbound : bâti sur Pokémon FireRed (code jeu `BPRE`, confirmé par recoupement avec
  une source publique indépendante et avec nos propres notes de Session 1).
- Que deux des sept signaux statistiques fournis (cibles ARM BL, signatures de compression) sont
  probablement dominés par des faux positifs à l'échelle de la ROM entière — démontré empiriquement, pas
  supposé.
- Qu'un index de "chaînes ASCII" dérivé d'une ROM Pokémon GBA ne peut structurellement pas représenter le
  texte de dialogue réel du jeu, à cause de l'encodage charmap propriétaire — conclusion méthodologique
  généralisable au-delà de ce seul pack.
- Une cartographie régionale (2048 régions de 16 Ko) par densité relative de signal, avec classification
  et score de confiance pour chacune.

## 2. Quelles structures sont très probablement identifiées ?

Aucune avec une confiance "PROBABLE" au niveau structure précise. La liste la plus solide reste au niveau
HYPOTHÈSE (voir `confidence.md`) : quelques séquences à pas constant dont la taille coïncide avec des
conventions publiques de struct (espèces ~28-36 octets, attaques ~32 octets), quelques régions à très
haute entropie candidates pour des données compressées.

## 3. Quelles structures restent inconnues ?

La quasi-totalité : système de cartes précis, système de scripts (bytecode), système de combat, contenu
des tables Pokémon/attaques/objets/dresseurs, tout dialogue réel, tout graphisme, toute donnée audio.
Voir `maps.md`, `scripts.md`, `battle-system.md`, `gameplay-systems.md`.

## 4. Comment fonctionne probablement le moteur ?

Par héritage architectural FireRed (PROBABLE, pas vérifié directement) : moteur ARM7TDMI mixte ARM/Thumb,
compression BIOS standard (LZ77/Huffman/RLE), et très probablement les mêmes grandes familles de systèmes
que celles documentées publiquement par `pret/pokefirered` (cartes/tilesets, scripts à bytecode, moteur de
combat par tours). Ceci est une inférence par analogie avec une base publique connue, pas une découverte
issue de l'analyse des CSV eux-mêmes.

## 5. Quelles parties peuvent être réimplémentées ?

Aucune "partie d'Unbound" à proprement parler — nous n'avons identifié aucun système avec assez de
précision pour le réimplémenter fidèlement. Ce qui est réimplémentable, c'est la **méthode** développée
ici (corrélation multi-signal, calibration empirique de seuils) comme outil de QA pour notre propre ROM.
Voir `project-recommendations.md`.

## 6. Quelles parties sont trop incertaines ?

Tout ce qui a été classé HYPOTHÈSE ou INCONNU dans `confidence.md` — soit l'essentiel du document. En
particulier, toute identification de fonction de code précise (aucune "fonction centrale" n'a pu être
confirmée, seulement des candidats par volume de références).

## 7. Quelles informations supplémentaires permettraient de progresser ?

- Un désassemblage réel (nécessiterait les octets bruts, donc la ROM — hors de portée de cette analyse
  par choix, pas par manque d'outils).
- Le contenu réel des chaînes/tables candidates (actuellement offsets et tailles seulement).
- Un accès légal au code source d'Unbound/CFRU s'il est un jour rendu public, plutôt qu'à son binaire.

## 8. Quelles sont les 20 découvertes les plus importantes ?

1. Le pack ne contient aucune ROM, image ou son — vérifié, pas supposé.
2. Unbound est bâti sur FireRed (`BPRE`), pas Émeraude.
3. Ce fait recoupe indépendamment nos notes de Session 1 sur ce même projet.
4. Le signal `arm-bl-targets.csv` est présent dans 100 % des 2048 régions — signe de saturation par faux
   positifs.
5. La cible ARM la plus référencée n'a que 13 références (vs 1345 pour la meilleure cible Thumb) —
   absence de "fonction hub", cohérent avec du bruit.
6. La région 0 (en-tête + début de code, structurellement non compressée) affiche 359 signatures de
   compression candidates — le signal compression est lui aussi partiellement bruité.
7. `string-index.csv` a une longueur médiane de 5 octets — incompatible avec du texte de dialogue réel.
8. Les jeux Pokémon GBA encodent le texte via une charmap propriétaire, pas de l'ASCII — donc un scanner
   ASCII générique manque presque entièrement le vrai texte du jeu.
9. Les régions classées "table_texte_probable" par la première passe de classification étaient en fait
   pour la plupart mal classées (seuil non calibré) — corrigé en seconde passe.
10. Après recalibration sur percentiles empiriques, la classification `code_thumb_probable` ne ressort
    presque jamais — limite de méthode documentée plutôt que "découverte" que le jeu a peu de code.
11. Deux candidats de table "espèce" (28 octets × 294, 36 octets × 298) coïncident en taille avec les
    conventions publiques pret — non confirmé, mais suggestif.
12. Un candidat de table "attaques" (32 octets × 136) est probablement une sous-table partielle vu le
    faible nombre d'enregistrements.
13. Les régions à plus haute entropie (jusqu'à 7.80/8.0) se recoupent partiellement avec des régions à
    forte densité de chaînes — chevauchement non résolu entre "données compressées" et "texte".
14. Les régions candidates "table de pointeurs" les plus denses (hors région d'en-tête) se regroupent
    dans une plage resserrée (~0x230000–0x3E7FFF).
15. Les régions candidates "table de texte" (au sens révisé, cf. #9) se regroupent aussi dans une plage
    resserrée (~0x117C000–0x1483FFF).
16. Le padding détecté (65 régions) se concentre presque exclusivement dans ~0x164000–0x1AFFFF.
17. Aucune structure de carte, de script ou de combat n'a pu être identifiée avec ce pack — limite de
    nature des données, pas d'effort d'analyse.
18. La méthode de corrélation développée (region-profile.csv) est réutilisable sur nos propres builds.
19. Trois refus d'analyse de ROM ont précédé cette étape ; ce pack a été vérifié avant traitement plutôt
    que rejeté ou accepté sur la seule foi de son étiquette.
20. La recommandation actionnable la plus solide pour notre projet n'est pas technique mais
    méthodologique et de discipline de contenu — voir `project-recommendations.md`.

## Limites de l'analyse

Voir `README.md` (section "Limites") et `confidence.md` pour le détail complet. En une phrase : cette
analyse caractérise correctement ce qu'un pack de statistiques binaires peut et ne peut pas révéler sans
accès au contenu — et le "ne peut pas" domine largement le résultat, honnêtement rapporté comme tel.

## Prochaines étapes recommandées

1. Décider avec Thomas si le sujet Unbound se referme ici, ou si l'intérêt réel porte sur son game design
   (auquel cas une discussion basée sur des sources publiques, pas du binaire, est proposée).
2. Reprendre le développement du projet (Arc 3/Ondine/Azuria, ou la liste d'items en attente dans
   `docs/PROJECT_STATUS.md`).
3. Si utile : généraliser `tools/unbound_analysis/*.py` en un outil d'audit de notre propre ROM compilée
   (cf. `project-recommendations.md`).
