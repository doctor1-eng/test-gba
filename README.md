# Contes du Donjon

Roguelike deckbuilder dark fantasy inspiré de *Dungeon Tales* dans son gameplay, avec l'ambiance visuelle et sonore de *Darkest Dungeon* (palette sombre rouge/noir/or, texte narratif macabre, tension psychologique).

PWA installable sur iPhone, jouable **100% hors-ligne** après le premier chargement. Aucune dépendance backend, aucun appel réseau après l'installation.

## Aperçu du gameplay

- Composez un groupe de 3 héros parmi 4 archétypes (Le Croisé, L'Hérétique, La Peste, Le Bourreau — ce dernier se débloque via la méta-progression).
- Combats au tour par tour avec un deck **partagé** entre les héros du groupe : chaque carte piochée appartient à un héros précis, qui l'exécute quand elle est jouée.
- Système de **stress** en plus des PV, façon Darkest Dungeon : à 100 de stress, un héros bascule en **affliction** (quirk négatif) ou **vertu** (quirk positif). Un héros à 0 PV entre à l'**article de la mort** — le coup suivant peut être fatal (probabilité de survie/mort).
- Progression à travers 2 zones (Le Hameau Oublié, Les Catacombes) de 7 étages chacune, avec boss de fin de zone, événements narratifs à choix, camps de repos, et récompenses de cartes après chaque combat.
- **Méta-progression** entre les runs : monnaie persistante (Reliques Anciennes) gagnée à chaque expédition, utilisable pour débloquer le 4ᵉ héros et des bonus de départ permanents.
- Sauvegarde automatique en `localStorage` : run en cours (y compris en plein combat) et historique des runs précédents.

## Stack technique

- **React 19 + TypeScript + Vite**
- **Zustand** pour l'état global, **Immer** pour les transitions de combat immuables
- **vite-plugin-pwa** (Workbox, stratégie cache-first) pour le fonctionnement hors-ligne
- Audio synthétisé en direct via **Web Audio API** (aucun fichier audio à précacher)
- Aucune dépendance backend, aucun appel réseau après le premier chargement

### Structure du projet

```
src/
  engine/       logique de jeu pure (aucun import React) : combat, stress, deck, rng, run, meta, audio
  data/         contenu du jeu en TypeScript typé : héros, cartes, ennemis, zones, events, quirks, reliques
  store/        store Zustand (orchestration) + persistance localStorage
  components/
    screens/    écrans (menu, sélection de héros, carte du donjon, combat, event, camp, etc.)
    combat/     composants de combat (panneaux héros/ennemis, main de cartes, log)
    ui/         primitives réutilisables (boutons, barres de PV/stress, modale)
    layout/     structure générale de l'app (barre du haut, safe-area)
  styles/       tokens de design (palette, typographie) et animations
scripts/
  generate-icons.mjs   régénère les icônes PWA/iOS à partir d'un SVG généré en code (via sharp)
```

La séparation `engine/` (logique pure, testable indépendamment) / `data/` (contenu éditable) / `components/` (UI) permet d'ajuster l'équilibrage ou d'ajouter du contenu sans toucher au rendu, et inversement.

## Contenu de ce premier build

- 4 héros jouables (3 débloqués par défaut, 1 en méta-progression)
- 20 cartes (16 propres aux héros + 4 cartes neutres trouvables en récompense)
- 10 types d'ennemis + 2 boss de zone
- 2 zones de 7 étages chacune (14 étages au total : combats, événements, camps, boss)
- 7 événements narratifs macabres à choix multiples et conséquences pondérées
- 8 quirks (4 afflictions, 4 vertus)
- 5 reliques
- 4 améliorations de méta-progression

## Développement

```bash
npm install
npm run dev       # serveur de développement (http://localhost:5173)
npm run build     # build de production dans dist/
npm run preview   # sert le build de production localement
npm run lint       # oxlint
npm run generate-icons   # régénère les icônes PWA (nécessite sharp, déjà en devDependency)
```

Le build de production génère un service worker (stratégie cache-first) qui précache tous les assets de l'application : une fois l'app chargée une première fois, elle fonctionne entièrement hors connexion.

## Installer sur iPhone (Safari)

1. Déployez le build de production (`npm run build`, dossier `dist/`) sur un hébergeur statique HTTPS (Vercel, Netlify, GitHub Pages, etc.), ou servez-le en local sur le réseau du téléphone via `npm run preview -- --host`.
2. Ouvrez l'URL du jeu dans **Safari** sur l'iPhone (l'installation en PWA ne fonctionne qu'avec Safari, pas Chrome iOS).
3. Laissez la page se charger entièrement une première fois (c'est ce chargement qui précache le jeu pour l'usage hors-ligne).
4. Appuyez sur le bouton **Partager** (icône carré avec flèche vers le haut) dans la barre d'outils de Safari.
5. Faites défiler et sélectionnez **« Sur l'écran d'accueil »**.
6. Confirmez le nom (« Contes du Donjon ») et appuyez sur **Ajouter**.
7. Lancez le jeu depuis l'icône sur l'écran d'accueil : il s'ouvre en plein écran (sans barre Safari) et fonctionne désormais même en mode avion.

**Notes iOS :**
- Le mode avion peut être activé dès l'étape 6 : tant que le premier chargement complet a eu lieu, tout le contenu (code, données, icônes) est en cache.
- La sauvegarde de partie utilise `localStorage`, propre à cette installation sur l'écran d'accueil. iOS peut occasionnellement purger le stockage des onglets Safari ouverts en navigation normale sous pression mémoire ; une fois l'app ajoutée à l'écran d'accueil et lancée depuis son icône, ce risque est fortement réduit.
- Le son démarre au premier tapotement dans l'app (contrainte standard iOS sur l'audio, gérée automatiquement).

## Limites connues de ce premier build

- Le nombre aléatoire utilisé en combat n'est pas persisté de façon strictement déterministe à travers un rechargement de page (léger impact sur la reproductibilité exacte d'un combat repris après fermeture de l'app, aucun impact sur la jouabilité).
- Les illustrations sont volontairement stylisées en CSS (glyphes, dégradés, vignettage façon gravure) plutôt qu'en images bitmap, pour rester légères et 100% hors-ligne sans dépendance à un service externe.
- Les écrans de démarrage (splash screen) iOS utilisent le rendu automatique d'iOS à partir du manifest plutôt que des images dédiées par modèle d'appareil.
