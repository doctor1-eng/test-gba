# EPISODES.md

## Arc 1 — Bourg Palette

### Épisode 1.1 — Réveil et découverte — **fait, Session 4**
- Emplacement : maison du joueur, Bourg Palette
- Personnages : Joueur, Maman
- Événements : réveil (le joueur habite déjà Bourg Palette, plus d'arrivée en camion), dialogue avec
  Maman (accueil + annonce que le professeur Chen l'attend au labo + "présentation du village" en une
  ligne), direction vers Route 1
- Maps concernées : `LittlerootTown_BrendansHouse_1F/2F` et `LittlerootTown_MaysHouse_1F/2F` (miroir
  selon le genre du joueur), `LittlerootTown` (suppression de la scène du camion), `InsideOfTruck`
  (contournée, plus jamais chargée)
- Scripts : intrigue "emménagement/papa champion d'arène" entièrement retirée (camion, cartons, horloge
  à régler de force, bulletin TV de l'Arène d'Argenta) ; remplacée par un dialogue unique de Maman
  (`PlayersHouse_1F_EventScript_BonjourMaman`, `data/scripts/players_house.inc`)
- **Non traité cette session** (contenu apparenté découvert en cours de route, cf. CHANGELOG) : la maison
  du rival (Régis) contient une scène "nouveau voisin" avec les mêmes références père/déménagement,
  toujours techniquement accessible indépendamment de ce qu'on vient de corriger

### Épisode 1.2 — Rencontre avec le Professeur Chen
- Emplacement : labo du Professeur Chen (ex-labo Birch)
- Personnages : Joueur, Professeur Chen
- Événements : discours d'ouverture (**fait**, Session 1), remise de Pikachu (starter unique, pas de choix)
- Pokémon : Pikachu (niveau 5, cohérent avec standard early-game)
- Maps concernées : `LittlerootTown_ProfessorBirchsLab` (à renommer)
- Scripts : à réécrire — remplacer la logique `starter_choose.c` par une remise directe

### Épisode 1.3 — Premier contact difficile avec Pikachu
- Emplacement : sortie du labo / Route 1
- Personnages : Joueur, Pikachu
- Événements : Pikachu refuse la Poké Ball, doit être suivi hors-balle un temps (mécanique à valider techniquement)
- Conséquence : pose le "Pikachu-personnage" dès le départ

### Épisode 1.4 — Première rencontre Team Rocket
- Emplacement : Route 1 ou abords de Bourg Palette
- Personnages : Jessie, James, Miaouss
- Événements : tentative de vol de Pokémon avortée, combat simple, fuite comique
- Fonction narrative : établit le motif récurrent Team Rocket dès l'Arc 1

### Épisode 1.5 — Départ de Bourg Palette
- Emplacement : sortie vers Route 1
- Personnages : Joueur, Maman (au revoir), Régis (première provocation)
- Événements : premier point de contact avec Régis, ouverture du monde

**Statut global Arc 1** : conception terminée (ce document), implémentation technique non commencée
hormis 1.2 (texte du discours). Prochaine étape : construire 1.1 → 1.5 dans le moteur (vertical slice,
cf. cahier des charges Phase 2).

---

## Arc 2 — Premières routes (squelette, non détaillé)
- Jadielle, forêt de Jade, Route 2-3
- Premiers combats de dresseurs, première arène (à définir laquelle en premier)

## Arc 3 — Ondine (squelette)
## Arc 4 — Pierre (squelette)
## Arc 5 — Kanto, suite (squelette)

*Ces arcs seront détaillés au même niveau que l'Arc 1 juste avant leur implémentation, pas avant —
principe d'optimisation des tokens du cahier des charges (§29) : ne pas produire de contenu qui ne sera
pas utilisé immédiatement.*
