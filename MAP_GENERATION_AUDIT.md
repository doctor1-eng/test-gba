# MAP_GENERATION_AUDIT.md

Audit préalable à l'implémentation du générateur de cartes Pokémon.
Réalisé avant toute génération, conformément au workflow obligatoire.

## 1. État du projet au démarrage

Le dépôt `test-gba` était **vide** au sens fonctionnel : un seul fichier,
`README.md` (`# test-gba`), aucun commit d'engine, aucun asset, aucun outil.

Conséquence directe sur la méthode : les sections 1 à 17 de la mission
demandent d'abord d'auditer l'existant pour "ne jamais recréer un système
qui existe déjà". Ici il n'existe rien à réutiliser — ni moteur GBA, ni
format de map, ni tileset, ni sprites, ni système de combat/sauvegarde/PNJ.
Ce document sert donc à la fois d'audit (négatif : rien à trouver) et de
proposition d'architecture, comme prévu section 21 quand les informations
manquent : "inspecte le code et les fichiers avant de prendre une décision"
— fait — puis décide.

- Moteur utilisé : **aucun**.
- Format de map : **aucun**.
- Résolution / taille de tiles : **aucune définie**.
- Tilesets / palettes / sprites : **aucun asset présent**.
- Collisions / warps / scripts / événements : **aucun système**.
- Sauvegarde / rencontres / combats / PNJ : **aucun système**.
- Outils de création de map : **aucun**.

## 2. Décision d'architecture (puisqu'il fallait choisir)

Le nom du dépôt (`test-gba`) suggère une cible Game Boy Advance. Compiler un
vrai ROM GBA (toolchain devkitARM, linkage ELF→GBA, moteur C bas niveau) est
un projet à part entière et n'est pas testable/vérifiable dans cet
environnement (pas d'émulateur graphique, pas de toolchain ARM). J'ai donc
choisi de construire le **pipeline de génération de cartes** comme un moteur
de données + outils, indépendant du runtime final, mais **strictement
contraint aux règles techniques de la GBA / Pokémon Gen 3** pour rester
portable vers un vrai moteur (GBA homebrew, Godot, Phaser, etc.) plus tard :

- Résolution écran : **240×160 px** (résolution native GBA).
- Tuile de base : **16×16 px** (taille des metatiles Pokémon Gen 3), donc
  **15×10 tuiles visibles** à l'écran.
- Grille de map en tuiles, pas en pixels libres (comme pokeemerald/pokered).
- Modèle de données inspiré du format interne de pokeemerald : `metadata`,
  `terrain` (layer de tuiles), `collision` (grille de solidité), `warps`,
  `objects`/NPCs (« object events »), `connections` (map voisines), table de
  rencontres par méthode (`grass`, `water`, `cave`, `fishing`).

Stack technique retenue pour le **générateur** (pas le jeu final) :

- **Node.js + TypeScript** : facile à exécuter, tester et valider ici,
  aucune compilation native requise.
- **Zod** : validation runtime stricte du schéma de map (garde-fou contre
  les maps techniquement invalides).
- **@napi-rs/canvas** : rendu des `preview.png` (binaire préconstruit, pas
  de toolchain C requise, contrairement au paquet `canvas` classique).
- **Commander** : CLI (`generate-map`, `validate-map`, `preview-map`,
  `generate-world`).
- **Vitest** : tests (déterminisme du générateur, non-régression du
  validateur, cohérence des connexions inter-maps).

Le format `map.json` produit est indépendant du moteur de rendu final : il
peut être consommé par un vrai firmware GBA (via un export ultérieur vers
des tableaux C) ou par un moteur web/Godot pour prototypage rapide. Ce choix
respecte la règle « ne jamais mélanger arbitrairement les styles » en gardant
le modèle de données strictement fidèle aux contraintes GBA même si le
rendu de preview est fait sur PC.

## 3. Assets graphiques : ce qui manque précisément

Aucun tileset, sprite ou palette n'existe dans le dépôt. Plutôt que de
générer immédiatement du pixel art incohérent (interdit section 17), le
générateur utilise un **tileset placeholder sémantique** :

- `assets/tilesets/*.json` : chaque tuile a un `id`, une `category`
  (`grass`, `path`, `tree`, `water`, `sand`, `wall`, `floor`, `door`, …),
  un `solid` (collision), une `color` (aplat RGB) et un `label`. Les
  previews rendent ces catégories en aplats de couleur + icônes simples,
  jamais des motifs aléatoires incohérents.
- Ceci permet de valider **structure, lisibilité, circulation, densité**
  (section 14) sans dépendre d'un artiste.

**Éléments manquants identifiés, à fournir pour la passe graphique finale :**

1. Feuilles de tuiles 16×16 px, palette ≤16 couleurs/palette (contrainte
   GBA Mode 0 tilemap), une par tileset (`overworld`, `cave`, `interior`).
2. Sprites de PNJ (marche 4 directions, 16×32 px, style Gen 3).
3. Sprites d'objets interactifs (Poké Ball au sol, panneaux, machines).
4. Palette de couleurs officielle du projet (aucune n'existe encore).

Le générateur référence ces assets par `id` logique dès maintenant, afin
qu'un remplacement par de vrais tilesets n'impacte que `assets/tilesets/*`,
jamais la logique de génération.

## 4. Systèmes de gameplay (combat, sauvegarde, etc.)

Absents du dépôt et **hors périmètre de cette mission** (qui porte sur la
génération de cartes). Le format de map expose cependant les points
d'intégration nécessaires pour qu'un futur moteur de combat/sauvegarde
s'y branche sans redesign :
- `encounters` par map → point d'entrée pour un système de combat sauvage.
- `events` avec `condition`/`flag` → point d'entrée pour un système de
  progression/sauvegarde (flags persistants).
- `npcs[].dialogue` → point d'entrée pour un moteur de dialogue/script.

## 5. Architecture retenue

```
test-gba/
├── MAP_GENERATION_AUDIT.md
├── src/
│   ├── schema/          # Types + validation Zod du format de map
│   ├── data/             # Datasets curés : espèces, archétypes PNJ, objets
│   ├── generator/         # Pipeline de génération (12 étapes, section 16)
│   │   ├── terrain/        # 1 module par biome (town, route, forest, cave, mountain, beach, village)
│   │   ├── rng.ts           # PRNG à seed (déterminisme)
│   │   ├── paths.ts, buildings.ts, decorations.ts, npcs.ts,
│   │   │ objects.ts, encounters.ts, warps.ts, landmarks.ts
│   │   └── index.ts         # orchestrateur
│   ├── validator/         # Rapport MAP VALIDATION (section 13)
│   ├── preview/            # Rendu PNG (section 14)
│   ├── world/               # Registre + graphe de connexions (section 11)
│   └── cli/                  # generate-map / validate-map / preview-map / generate-world
├── templates/            # Bibliothèque de templates (section 15)
│   ├── town/ village/ route/ forest/ cave/ mountain/ beach/
│   ├── pokemon_center/ pokemart/ gym/ interior/ special/
├── assets/tilesets/      # Tilesets placeholder + doc des assets manquants
├── world/world.json      # Registre mondial des maps générées
├── maps/<map_id>/        # map.json, metadata.json, preview.png
└── tests/                # Vitest : déterminisme, validateur, connexions
```

## 6. Plan d'implémentation

1. Schéma de données (`src/schema`) + validation Zod.
2. Datasets placeholder (tilesets, archétypes PNJ, objets, espèces par biome).
3. Bibliothèque de templates (règles par biome/bâtiment, pas de valeurs
   codées en dur dans le générateur).
4. Primitives de génération (RNG seedé, tracé de chemins, flood-fill de
   connectivité, placement contraint).
5. Générateurs de terrain par biome (7 extérieurs + 5 intérieurs), chacun
   répondant aux 10 questions de la section 18 via les métadonnées du
   template (intention de zone, landmarks, progression).
6. Placement PNJ / objets / décorations / rencontres, piloté par règles.
7. Warps + registre monde (connexions bidirectionnelles vérifiées).
8. Validateur (technique / gameplay / design) + rapport formaté.
9. Rendu preview PNG.
10. CLI (`generate-map`, `validate-map`, `preview-map`, `generate-world`).
11. Démonstration : petit monde connecté (ville de départ → route → forêt,
    avec Centre Pokémon généré en intérieur) + validation finale de chaque
    carte.
12. Tests automatisés + vérification que `npm run build`/`test` passent.

## 7. Limites assumées de cette V1

- 7 biomes extérieurs et 5 types d'intérieurs sont implémentés avec de
  vraies règles procédurales contraintes (pas de placement aléatoire nu),
  mais chaque algorithme reste volontairement simple/lisible plutôt
  qu'exhaustif — conçu pour être affiné itérativement (section 20 : livrer
  puis corriger plutôt que bloquer sur un absolu).
- Le jeu de données Pokémon (espèces par biome) est un sous-ensemble curé
  et documenté comme extensible, pas un Pokédex complet.
- Aucun rendu graphique final (vrais tilesets) : le pipeline est prêt à les
  recevoir dès qu'ils existent (voir section 3).
