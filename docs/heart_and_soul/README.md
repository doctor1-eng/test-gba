# Heart & Soul — Brief d'implémentation pour Claude Code

## Contexte
Fork de **pokeemerald-expansion**, région de Kanto en monde ouvert. Dépôt cible : `doctor1-eng/test-gba`, branche `claude/pokemon-heart-soul-audit-qv0f4n`. L'histoire complète, les personnages, les mécaniques narratives et les systèmes (réputation, conviction des lieutenants, aspirants Champions) sont détaillés dans `docs/histoire.md`. Ce fichier README sert de point d'entrée : lis-le en premier, puis réfère-toi à `docs/histoire.md` pour tout détail narratif, et à `scripts/` pour les templates Poryscript déjà rédigés (génériques, à adapter aux vraies constantes du repo).

**Important — état de la branche cible** : ce dossier a été préparé hors du repo (pas d'accès direct au dépôt lors de sa création). Avant toute action, compare le contenu de ce dossier à ce qui existe déjà sur la branche `claude/pokemon-heart-soul-audit-qv0f4n` — si des fichiers équivalents y sont déjà présents (créés par une session Claude Code antérieure), ne les écrase pas silencieusement : signale les différences et propose une fusion plutôt qu'un remplacement.

## Comment travailler sur ce projet
1. **Toujours vérifier les constantes réelles du repo avant d'adapter un template** : noms de maps (`include/constants/map_groups.h` ou équivalent), espèces (`include/constants/species.h`), flags/vars disponibles, syntaxe exacte des commandes Poryscript de cette version (`givemon`, `multichoice`, `trainerbattle`, etc. peuvent varier selon la version du fork).
2. **Ne pas tout scripter d'un coup** : suivre l'ordre de priorité ci-dessous, act par acte, et valider chaque bloc avant de passer au suivant.
3. **Rester fidèle au ton "sombre et réaliste"** décrit dans `docs/histoire.md` section 1 — éviter tout dialogue trop léger ou trop héroïque dans les scripts générés.

## Ordre de priorité

| Élément | Statut | Fichier |
|---|---|---|
| Choix du type + sélection de 4 Pokémon niv. 34 | ✅ Template prêt (Feu en exemple complet, 17 types à dupliquer) | `scripts/intro_choix_type.pory` |
| Attaque de Cinnabar (Acte I) + choix des réfugiés | ✅ Template prêt | `scripts/cinnabar_acte1.pory` |
| Séquence de conviction Terrence (Route de la Centrale) | ✅ Template prêt | `scripts/terrence_conviction.pory` |
| Séquence de conviction Kess (Zone Safari) | ✅ Template prêt | `scripts/kess_conviction.pory` |
| Combats des lieutenants Lyre, Selen, Mira Voss | ✅ Template prêt | `scripts/lieutenants_combat.pory` |
| Dialogue final face à Blue (Acte V, 3 choix) + branchement réputation | ✅ Template prêt | `scripts/blue_final.pory` |
| Module de réputation (vérification de seuil, branchement épilogue) | ✅ Intégré dans `blue_final.pory` (addvar) et `epilogues.pory` (seuils) | `scripts/blue_final.pory`, `scripts/epilogues.pory` |
| 3 épilogues (ternie / équilibrée / exemplaire) | ✅ Template prêt, textes complets scriptés | `scripts/epilogues.pory` |
| Boss post-game Commandant Kaïn | ✅ Template prêt | `scripts/postgame_kain.pory` |
| Quêtes annexes par ville/route (13 lieux, squelette fonctionnel) | ✅ Template prêt, à enrichir en dialogues complets | `scripts/side_quests.pory` |
| 18 événements narratifs supplémentaires (Actes I-V) | ⬜ Non scripté — voir `docs/histoire.md` section 11 | — |
| 8 aspirants Champions en rencontre aléatoire | ⬜ Non scripté — voir `docs/histoire.md` section 12 | — |

**Ce qui reste un vrai travail d'intégration** (pas de la conception, mais du câblage technique) :
- Remplacer toutes les constantes `MAP_*`, `SPECIES_*`, `TRAINER_*`, `MULTI_*`, `SE_*` par les vraies valeurs du repo.
- Déclarer tous les `FLAG_*` et `VAR_*` listés dans `include/constants/flags.h` / `vars.h`.
- Dupliquer `EventScript_Liste_Feu` (dans `intro_choix_type.pory`) pour les 17 autres types à partir des tableaux de la section 6 de `docs/histoire.md`.
- Créer les données de combat (`trainers.party` ou équivalent) pour chaque dresseur nommé, en respectant les niveaux indiqués (34 pour la trame principale, 55-60 pour Kaïn en post-game).
- Décider et scripter où/quand `FLAG_BLAINE_SAUVE` est posé (Acte IV ou V, non détaillé dans les templates) — condition son texte d'épilogue.
- Scripter les 18 événements (section 11) et le système des 8 aspirants (section 12), en suivant le même modèle que les fichiers existants (flags de progression, `addvar` sur `VAR_REPUTATION` quand applicable).

## Références rapides dans `docs/histoire.md`
- Section 6 : les 18 tableaux de Pokémon par type (niveau 34) — nécessaires pour dupliquer `EventScript_Liste_Feu` aux 17 autres types.
- Section 9 : barème complet du système de réputation (`VAR_REPUTATION`).
- Section 10 : textes des 3 épilogues et contenu post-game.
- Section 11 : 18 événements narratifs supplémentaires par acte (non scriptés).
- Section 12 : les 8 aspirants Champions en rencontre aléatoire (non scriptés).
- Section "Antagoniste principal et lieutenants" : profils, styles de combat et scènes de dialogue de Blue, Terrence, Kess, Lyre, Selen, Mira Voss.

## Ce que Claude Code doit faire en premier
1. Cloner/ouvrir la branche `claude/pokemon-heart-soul-audit-qv0f4n` du repo `doctor1-eng/test-gba` et comparer son contenu actuel à ce dossier — signaler les recoupements ou conflits avant d'écrire quoi que ce soit.
2. Explorer le repo pour identifier les vraies constantes (maps, espèces, flags/vars déjà utilisés) et les conventions de nommage du projet.
3. Adapter `scripts/intro_choix_type.pory` en conséquence, en dupliquant la structure pour les 17 types manquants (seul Feu est fait en exemple).
4. Adapter `scripts/cinnabar_acte1.pory` (vérifier notamment la gestion de visibilité de Blaine et les IDs de map réels).
5. Suivre l'ordre narratif (Acte I → Acte V → post-game → sections 11/12) pour adapter les fichiers restants, en vérifiant à chaque étape que les flags/vars utilisés sont cohérents entre les fichiers (ex. `FLAG_TERRENCE_CONVAINCU` posé dans `terrence_conviction.pory` est relu dans `epilogues.pory`).
6. Me signaler toute incohérence trouvée entre les templates et la structure réelle du repo plutôt que de la corriger silencieusement — certains choix (ex. noms de flags, seuils numériques) sont issus de décisions de conception documentées dans `docs/histoire.md` et ne doivent pas être modifiés sans validation.

