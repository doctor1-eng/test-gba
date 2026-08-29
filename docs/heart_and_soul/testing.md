# Heart & Soul — Checklist de tests

Cochée uniquement quand build + test manuel en jeu (mGBA ou équivalent) ont été effectués,
pas sur la seule base de la compilation.

- [x] Build baseline (`make hns`, fork non modifié) — PASS, 0 erreur, 430 warnings préexistants
- [x] Build avec vertical slice (registre flags/vars + intro + Acte I) — PASS, 0 erreur nouvelle
- [x] Intro / choix du type, type Feu — **joué et confirmé par l'utilisateur** : sélection des 4 Pokémon Feu fonctionnelle, choix des attaques (Move Relearner) fonctionnel, chaussures de course actives.
- [ ] Intro / choix du type, 17 autres types — compilé, PASS (`make hns -j4`, 0 erreur) après extension du mécanisme Feu à tous les types ; **non joué en émulateur par personne** (généré et vérifié par compilation + relecture, pas testé en jeu). À confirmer par l'utilisateur type par type ou par sondage.
- [ ] Cinnabar (Acte I) — compilé, PASS ; **non joué en émulateur**. Verrouillage de Cinnabar posé en flag mais pas encore appliqué au retour (voir implementation_notes.md).
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
