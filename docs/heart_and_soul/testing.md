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
- [ ] Acte II — Pierre/Ondine doutent avant le combat de badge (`heart_and_soul_act2.inc`) — compilé, PASS ; **non joué en émulateur**.
- [ ] Argenta (Viridian) — Blue masqué, Arène fermée — compilé, PASS ; **non joué en émulateur**.
- [ ] Route 1 — objet perdu / milice (Quinn) — **retour utilisateur : le dialogue post-combat ne s'est pas déclenché**. Relecture du script : structure identique au motif déjà prouvé (Act I). Hypothèse la plus probable : sauvegarde ayant déjà dépassé la scène de fuite de Cinnabar avant que `FLAG_OBJET_PERDU_TROUVE` existe dans le ROM testé, donc jamais posé pour cette sauvegarde (voir implementation_notes.md, retour n°7). **À retester sur une sauvegarde neuve en priorité absolue** — c'est le test qui doit trancher si c'est un vrai bug de code ou un artefact de continuité de sauvegarde.
- [x] Confort de test — discours du Professeur Chen sauté, 5 Poké Balls de départ — compilé, PASS ; à confirmer que l'écran de choix du genre s'affiche correctement juste après l'écran-titre (premier point à vérifier avant tout le reste, puisque tout test futur en dépend).
- [ ] Viridian, commerçant résistant (Mart) — compilé, PASS ; **non joué en émulateur**.
- [ ] Forêt de Jade, bûcherons déplacés (Doug) — compilé, PASS ; **non joué en émulateur**. Même point d'attention que Route 1 (post-combat `trainerbattle_single`).
- [ ] Azuria, canalisations sabotées (Boy) — compilé, PASS ; **non joué en émulateur**.
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
