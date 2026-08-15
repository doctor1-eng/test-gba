# test-gba

Pipeline de génération de cartes Pokémon (style GBA / Gen 3) : générateur
procédural contraint par des règles de level design, validateur automatique,
previews PNG. Voir [MAP_GENERATION_AUDIT.md](./MAP_GENERATION_AUDIT.md) pour
l'analyse complète et l'architecture détaillée.

## Installation

```bash
npm install
```

## Commandes

```bash
# Génère une carte, la valide, produit une preview.
npx tsx src/cli/index.ts generate-map --type town --name "Bourg Départ"

# Options : --seed --region --progression (early|mid|late) --width --height
#           --directions north,south   (sorties non câblées, juste tracées)
#           --link south=route_1       (câble la sortie sud vers une map existante)

npx tsx src/cli/index.ts validate-map bourg_depart
npx tsx src/cli/index.ts preview-map bourg_depart
npx tsx src/cli/index.ts list-templates

# Génère le petit monde de démonstration connecté (ville → route → forêt,
# avec Centre Pokémon + maison en intérieurs), le valide de bout en bout.
npx tsx src/cli/index.ts generate-world
```

Raccourcis npm équivalents : `npm run generate-map -- --type town --name "..."`,
`npm run validate-map -- <id>`, `npm run preview-map -- <id>`, `npm run generate-world`.

## Vérifications

```bash
npm run typecheck   # tsc --noEmit
npm test             # vitest (déterminisme, validateur, cohérence des connexions)
```

## Structure

- `src/schema/` — format de map (types + validation Zod)
- `src/generator/` — pipeline de génération (terrain par biome, bâtiments,
  PNJ, objets, décorations, rencontres, landmarks, warps)
- `src/validator/` — vérifications technique / gameplay / design + rapport
- `src/preview/` — rendu PNG des previews
- `src/world/` — registre monde + cohérence des connexions inter-maps
- `templates/` — bibliothèque de templates par type de carte (règles, pas
  de valeurs codées en dur dans le générateur)
- `assets/tilesets/` — tilesets placeholder (voir `assets/MISSING_ASSETS.md`)
- `maps/<id>/` — sorties générées : `map.json`, `metadata.json`,
  `preview.png`, `validation_report.txt`
- `world/world.json` — registre des maps générées
