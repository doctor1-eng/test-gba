# Assets manquants

Voir MAP_GENERATION_AUDIT.md section 3 pour le contexte complet. Résumé
actionnable pour un artiste/technical artist :

| Asset | Spec attendue | Statut |
|---|---|---|
| Feuilles de tuiles overworld | 16×16 px, palette ≤16 couleurs, style Gen 3 | Manquant — placeholder couleur dans `tilesets/overworld.json` |
| Feuilles de tuiles cave | 16×16 px, palette sombre | Manquant — placeholder dans `tilesets/cave.json` |
| Feuilles de tuiles intérieur | 16×16 px | Manquant — placeholder dans `tilesets/interior.json` |
| Sprites PNJ (4 directions, marche) | 16×32 px, style Gen 3 | Manquant — voir `npc-archetypes.json` (couleur/role uniquement) |
| Sprites objets interactifs | 16×16 px | Manquant — voir `objects-catalog.json` |
| Palette officielle du projet | N/A | Non définie |

Chaque tuile/PNJ/objet est référencé par un `id` logique stable dans tout le
pipeline : remplacer un placeholder par un vrai asset ne touche que les
fichiers de `assets/`, jamais la logique de génération dans `src/generator`.
