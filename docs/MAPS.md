# Cartes

## Bourg Palette

Village de départ (type `village`, sans Centre Pokémon — cohérent avec le
village de départ original de la franchise). Données source :
`src/data/bourg-palette.json`, consommées par `src/world/demoWorld.ts` via
le paramètre `forcedBuildings` de `generateMap()` (voir `src/generator/buildings.ts`).

8 bâtiments scriptés (pas de tirage procédural) :

| # | Nom | Type |
|---|-----|------|
| 1 | Laboratoire du Professeur Chen | lab |
| 2 | Maison du joueur | house |
| 3 | Maison de Régis | house |
| 4 | Maison de Mme Chen | house |
| 5 | Maison du Vieux Dresseur | house |
| 6 | Maison du Gardien de Route | house |
| 7 | Maison aux volets fermés | house |
| 8 | Maison de la Dame aux Baies | house |

Connexion : sortie sud → Route 1 → Forêt Émeraude (voir `demoWorld.ts`).

Pour changer la liste des bâtiments, éditer `src/data/bourg-palette.json`
(pas `demoWorld.ts` ni `buildings.ts`).
