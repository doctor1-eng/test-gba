# Heart & Soul — Checklist de tests

Cochée uniquement quand build + test manuel en jeu (mGBA ou équivalent) ont été effectués,
pas sur la seule base de la compilation.

- [x] Build baseline (`make hns`, fork non modifié) — PASS, 0 erreur, 430 warnings préexistants
- [x] Build avec vertical slice (registre flags/vars + intro + Acte I) — PASS, 0 erreur nouvelle
- [ ] Intro / choix du type — compilé, PASS ; **non joué en émulateur** (pas d'accès écran dans cette session). Type Feu seul fonctionnel, 17 autres = message "bientôt disponible" + retour au menu.
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
