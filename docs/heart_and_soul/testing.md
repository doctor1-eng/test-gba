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
- [x] Acte II — Pierre doute avant le combat, se rejoue après une défaite — **confirmé par l'utilisateur** (« Fonctionne »).
- [ ] Acte II — Ondine, même correctif que Pierre (même cause) — jamais testée séparément par l'utilisateur, mais chemin de code identique ; même chose désormais pour Major Bob (4e Arène) — à confirmer.
- [x] Argenta (Viridian) — Blue masqué, Arène fermée — **confirmé par l'utilisateur**.
- [x] Route 1 — objet perdu / milice (Quinn), dialogue automatique après victoire — **confirmé par l'utilisateur** après le correctif `event_script`.
- [x] Viridian, commerçant résistant (Mart) — **confirmé fonctionnel par l'utilisateur**.
- [ ] Forêt de Jade, bûcherons déplacés (Doug) — même correctif que Quinn appliqué, mais **non testé séparément par l'utilisateur** (route empruntée différait lors du dernier test) — à confirmer en priorité.
- [ ] Azuria, canalisations sabotées (Boy) — pas encore atteint par l'utilisateur — à tester.
- [x] Pokémon sauvages relevés niveau 34 (Cinnabar→Azuria) — **confirmé par l'utilisateur**.
- [x] Capacités de terrain utilisables sans badge — **confirmé par l'utilisateur**.
- [ ] CT (Coupe/Vol/Surf/etc.) et Bicyclette données dès le départ — nouveau, compilé PASS, **non testé en jeu**. Le CS Vol en particulier était le point signalé cassé, à revérifier en priorité.
- [ ] Route vers Carmin-sur-Mer (Route 5 → Safrania → Route 6), dresseurs/sauvages niveau 34 — nouveau, compilé PASS, **non testé en jeu**.
- [ ] Carmin-sur-Mer, interception radio (PNJ Nerd) — nouveau, compilé PASS, **non testé en jeu**.
- [ ] Cannes à pêche (Old/Good/Super Rod) dans le sac dès le départ — nouveau, compilé PASS, **non testé en jeu**.
- [ ] Magasins d'Argenta/Jadielle/Azuria/Carmin-sur-Mer vendent réellement des objets (PNJ Clerk était mal câblé sur le vendeur de Cherrygrove/Johto) — nouveau, compilé PASS, **non testé en jeu**.
- [ ] Portes Arène/Manoir/Labo sur Cinnabar (travail d'une session parallèle, fusionné) — blockout compilé, PASS, **non testé en jeu** ; voir `MAP_TEST_README.md` pour un build de test dédié (`make hns MAPTEST=1`) qui démarre directement à côté des 3 portes.
- [ ] 4e Arène (Major Bob/Lt. Surge) — même schéma que Pierre/Ondine — nouveau, compilé PASS, **non testé en jeu**.
- [ ] Pallet Town, événement « le monde d'avant » (PNJ Woman) — nouveau, compilé PASS, **non testé en jeu**.
- [ ] **Traduction française complète Cinnabar → Vermilion** — chantier de ~1300 lignes réparti sur ~45 cartes, build PASS, balayage automatique ne trouve plus de texte anglais résiduel (sauf le bloc Blue à Argenta, volontairement laissé de côté car inaccessible avant l'Acte V). **Jamais testé visuellement en émulateur** — c'est le point qui demande le plus de vigilance : vérifier qu'aucune ligne ne déborde de sa fenêtre de dialogue.
- [ ] Rock Tunnel
- [ ] Safari
- [ ] Fuchsia
- [ ] Celadon
- [ ] Saffron (arène, Sabrina)
- [ ] Silph
- [ ] Blue
- [ ] Épilogues
- [ ] Post-game
- [ ] Kaïn

## Méthode

Après chaque script ajouté : `make hns`, puis test manuel des cas listés en section 19 du
brief (première interaction, interaction après résolution, bouton B dans les menus,
sauvegarde/rechargement, sortie/retour sur la map, flag conservé, réputation non doublée).
