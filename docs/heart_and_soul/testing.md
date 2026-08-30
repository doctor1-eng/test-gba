# Heart & Soul — Checklist de tests

Cochée uniquement quand build + test manuel en jeu (mGBA ou équivalent) ont été effectués,
pas sur la seule base de la compilation.

- [x] Build baseline (`make hns`, fork non modifié) — PASS, 0 erreur, 430 warnings préexistants
- [x] Build avec vertical slice (registre flags/vars + intro + Acte I) — PASS, 0 erreur nouvelle
- [x] Intro / choix du type, type Feu — **joué et confirmé par l'utilisateur** : sélection des 4 Pokémon Feu fonctionnelle, choix des attaques (Move Relearner) fonctionnel, chaussures de course actives.
- [ ] Intro / choix du type, 17 autres types — compilé, PASS (`make hns -j4`, 0 erreur) après extension du mécanisme Feu à tous les types ; **non joué en émulateur par personne** (généré et vérifié par compilation + relecture, pas testé en jeu). À confirmer par l'utilisateur type par type ou par sondage.
- [x] Menu START, entrée "Pokémon" — bug remonté par l'utilisateur (absente après le choix d'équipe), corrigé par `setflag FLAG_SYS_POKEMON_GET` dans les 18 `ChooseTeam_{type}` (voir implementation_notes.md). Compilé, PASS ; à reconfirmer par l'utilisateur en jeu.
- [x] Cinnabar (Acte I) — bug bloquant confirmé par l'utilisateur (fuite impossible sans CS Fly), corrigé (warp scripté vers Pallet Town + blocage du retour via ON_FRAME_TABLE, voir implementation_notes.md retour n°5). **Fuite/attaque confirmées fonctionnelles par l'utilisateur** (retour n°3, "Tout fonctionne"). Seul le blocage du retour physique à Cinnabar (`HeartSoul_EventScript_CinnabarVerrouilleeCheck`) reste **non encore testé en jeu par l'utilisateur** ("je n'ai tjs pas pu test le retour pour l'instant") — à confirmer en priorité.
- [ ] Cinnabar — warps des bâtiments (Arène/Manoir/Labo) — **pas fait** : nécessite une vérification visuelle de la tilemap (capture d'écran utilisateur) pour ne pas placer un warp sur une case incohérente avec le décor.
- [x] Confort de test — discours du Professeur Chen sauté — **confirmé par l'utilisateur**, l'écran de choix du genre s'affiche bien directement.
- [x] Acte II — Pierre doute avant le combat, se rejoue après une défaite — **confirmé par l'utilisateur** (« Fonctionne »). Retour n°8 : texte de base doublon retiré, texte H&S post-victoire ajouté — **compilé PASS, non encore testé en jeu**.
- [ ] Acte II — Ondine : mécanique retravaillée suite au retour n°9 (Arène verrouillée tant que `FLAG_CANALISATIONS_REPAREES` n'est pas posé, scène préalable avec Ondine, PNJ Rocket saboteur interactif puis retiré) — **nouveau, compilé PASS, non encore testé en jeu** ; Major Bob (4e Arène) toujours sur l'ancien schéma Pierre/Quinn (doute + combat), non retesté séparément.
- [x] Argenta (Viridian) — Blue masqué, Arène fermée — **confirmé par l'utilisateur**.
- [x] Route 1 — objet perdu / milice (Quinn), dialogue automatique après victoire — **confirmé par l'utilisateur** après le correctif `event_script`.
- [x] Viridian, commerçant résistant (Mart) — **confirmé fonctionnel par l'utilisateur**.
- [ ] Forêt de Jade, bûcherons déplacés (Doug) — **retour n°6 : "je ne trouve pas Doug"**. Investigué : pas de flag caché, PNJ toujours présent (`"flag": "0"`), mais il erre (wander) à `(29,58)`, assez loin de l'entrée nord, plutôt proche de la sortie sud vers Jadielle/Pewter, sur une map en labyrinthe — pas un bug identifié. Indication de position ajoutée à `quick_test_checklist.md`. À reconfirmer par l'utilisateur.
- [ ] Azuria, canalisations sabotées (Boy) — atteint par l'utilisateur (retour n°9), désormais un prérequis pour déverrouiller le combat d'Ondine (voir ci-dessus) — à confirmer avec la nouvelle mécanique.
- [x] Pokémon sauvages relevés niveau 34 (Cinnabar→Azuria) — **confirmé par l'utilisateur**.
- [x] Capacités de terrain utilisables sans badge — **confirmé par l'utilisateur**.
- [x] CT (Coupe/Vol/Surf/etc.) et Bicyclette données dès le départ — **confirmé par l'utilisateur** (retour n°2, "Tout fonctionne").
- [x] Route vers Carmin-sur-Mer (Route 5 → Safrania → Route 6), dresseurs/sauvages niveau 34 — **confirmé par l'utilisateur** (retour n°7/10, trajet parcouru sans blocage signalé).
- [x] Carmin-sur-Mer, interception radio (PNJ Nerd) — **confirmé par l'utilisateur** (retour n°10, "Pnj nerf fonctionne").
- [x] Cannes à pêche (Old/Good/Super Rod) dans le sac dès le départ — **confirmé par l'utilisateur** (retour n°2, "Tout fonctionne").
- [x] Magasins d'Argenta/Jadielle/Azuria/Carmin-sur-Mer vendent réellement des objets (PNJ Clerk était mal câblé sur le vendeur de Cherrygrove/Johto) — **confirmé par l'utilisateur** (retours n°5/7/9, "Tout fonctionne").
- [ ] Portes Arène/Manoir/Labo sur Cinnabar (travail d'une session parallèle, fusionné) — blockout compilé, PASS, **non testé en jeu** ; voir `MAP_TEST_README.md` pour un build de test dédié (`make hns MAPTEST=1`) qui démarre directement à côté des 3 portes.
- [x] 4e Arène (Major Bob/Lt. Surge) — même schéma que Pierre/Ondine — **confirmé par l'utilisateur** (retour n°10, "Tout fonctionne").
- [ ] Retour n°10 — 3 observations signalées "je ne sais pas si c'est normal", à clarifier avec l'utilisateur : (a) « Évent qui se lance sur l'agrandissement du parc safari dans le bâtiment d'entrée de Carmin-sur-Mer » — recherche par mot-clé (Vermilion, Port, Safari) infructueuse, contenu non localisé, besoin de précisions (quel bâtiment exactement, texte affiché) ; (b) « Tunnel bloqué de la Route 5 avec PNJ qui dit qu'il faut régler le problème power plant » — idem, recherche `POWER_PLANT`/`Route5_hns` infructueuse ; (c) événement Steven (Champion de Hoenn) en entrant à Carmin-sur-Mer — **confirmé intentionnel, contenu de base du jeu (crossover) préexistant, sans lien avec Heart & Soul, non cassé par ce travail**.
- [ ] Pallet Town, événement « le monde d'avant » (PNJ Woman) — nouveau, compilé PASS, **non testé en jeu**.
- [ ] Carte de Vol (écran « FLY to where? ») — **bug confirmé par capture d'écran utilisateur** (nom de ville correct mais géographie alentour incohérente, cause : `FLAG_VISITED_KANTO` jamais posé donc `GetMapSecIdAt()` restait sur la grille Johto seule, voir implementation_notes.md). Corrigé (`setflag FLAG_VISITED_KANTO` dans les 18 `ChooseTeam_{type}`), compilé PASS. **Ne s'applique qu'à une nouvelle partie** — la sauvegarde utilisée pour la capture d'écran doit être recommencée pour voir la correction. Limite connue non corrigée : l'écran Carte du menu START (différent de l'écran Vol) affiche toujours "JOHTO" en titre.
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
