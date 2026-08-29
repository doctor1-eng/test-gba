# Heart & Soul — Checklist de tests

Cochée uniquement quand build + test manuel en jeu (mGBA ou équivalent) ont été effectués,
pas sur la seule base de la compilation.

- [x] Build baseline (`make hns`, fork non modifié) — PASS, 0 erreur, 430 warnings préexistants
- [x] Build avec vertical slice (registre flags/vars + intro + Acte I) — PASS, 0 erreur nouvelle
- [x] Intro / choix du type, type Feu — **joué et confirmé par l'utilisateur** : sélection des 4 Pokémon Feu fonctionnelle, choix des attaques (Move Relearner) fonctionnel, chaussures de course actives.
- [ ] Intro / choix du type, 17 autres types — compilé, PASS (`make hns -j4`, 0 erreur) après extension du mécanisme Feu à tous les types ; **non joué en émulateur par personne** (généré et vérifié par compilation + relecture, pas testé en jeu). À confirmer par l'utilisateur type par type ou par sondage.
- [x] Menu START, entrée "Pokémon" — bug remonté par l'utilisateur (absente après le choix d'équipe), corrigé par `setflag FLAG_SYS_POKEMON_GET` dans les 18 `ChooseTeam_{type}` (voir implementation_notes.md). Compilé, PASS ; à reconfirmer par l'utilisateur en jeu.
- [ ] Cinnabar (Acte I) — bug bloquant confirmé par l'utilisateur (fuite impossible sans CS Fly), corrigé (warp scripté vers Pallet Town + blocage du retour via ON_FRAME_TABLE, voir implementation_notes.md retour n°5). Compilé, PASS ; **fuite non encore rejouée en émulateur après le correctif, à confirmer par l'utilisateur en priorité**.
- [ ] Cinnabar — warps des bâtiments (Arène/Manoir/Labo) — **pas fait** : nécessite une vérification visuelle de la tilemap (capture d'écran utilisateur) pour ne pas placer un warp sur une case incohérente avec le décor.
- [x] Confort de test — discours du Professeur Chen sauté — **confirmé par l'utilisateur**, l'écran de choix du genre s'affiche bien directement.
- [~] Acte II — Pierre doute avant le combat de badge — **retour utilisateur : notre dialogue s'affiche puis s'enchaîne sur le dialogue anglais d'origine avant le combat (attendu) ; mais après une défaite, seul l'anglais se relance (bug confirmé)**. Cause identifiée et corrigée (voir implementation_notes.md, retour n°8 : le flag de conviction était posé trop tôt, avant l'issue du combat). **À retester : la scène de doute doit maintenant se rejouer à chaque tentative tant que le combat n'est pas gagné.**
- [ ] Acte II — Ondine, même correctif que Pierre (même cause), jamais testée séparément — à confirmer.
- [ ] Argenta (Viridian) — Blue masqué, Arène fermée — compilé, PASS ; **non joué en émulateur**.
- [~] Route 1 — objet perdu / milice (Quinn) — **retour utilisateur : bug confirmé, pas un artefact de sauvegarde comme supposé** (« il faut lui parler après sa défaite, il y a un précédent dialogue en anglais puis notre dialogue »). Cause réelle identifiée dans `data/scripts/trainer_battle.inc` et corrigée (voir implementation_notes.md, retour n°8 : `trainerbattle_single` a besoin de son 4ᵉ argument `event_script` pour enchaîner dès la première victoire, sinon le code qui suit n'est atteint qu'au rappel). **À retester en priorité absolue.**
- [ ] Viridian, commerçant résistant (Mart) — **confirmé fonctionnel par l'utilisateur** (PNJ à interaction classique, pas de `trainerbattle`, donc pas concerné par le bug ci-dessus).
- [~] Forêt de Jade, bûcherons déplacés (Doug) — même bug que Route 1 (même mécanisme `trainerbattle_single`), même correctif appliqué. **Non testé séparément, à confirmer.**
- [ ] Azuria, canalisations sabotées (Boy) — compilé, PASS ; **non joué en émulateur**.
- [ ] Pokémon sauvages relevés niveau 34 (Cinnabar→Azuria) — compilé, PASS ; **non joué en émulateur**.
- [ ] Pokégear/carte donnée dès le départ — compilé, PASS ; **non joué en émulateur**.
- [ ] Capacités de terrain (Coupe/Surf/Vol/etc.) utilisables sans badge — compilé, PASS ; **non joué en émulateur** — nécessite d'avoir la CT et un Pokémon qui la connaît pour tester réellement.
- [ ] Route 21
- [ ] Route 1
- [ ] Viridian
- [ ] Pewter
- [ ] Cerulean
- [ ] Viridian Forest
- [ ] Mt Moon
- [ ] Rock Tunnel
- [ ] Vermilion
- [ ] Safari
- [ ] Fuchsia
- [ ] Celadon
- [ ] Saffron
- [ ] Silph
- [ ] Blue
- [ ] Épilogues
- [ ] Post-game
- [ ] Kaïn

## Méthode

Après chaque script ajouté : `make hns`, puis test manuel des cas listés en section 19 du
brief (première interaction, interaction après résolution, bouton B dans les menus,
sauvegarde/rechargement, sortie/retour sur la map, flag conservé, réputation non doublée).
