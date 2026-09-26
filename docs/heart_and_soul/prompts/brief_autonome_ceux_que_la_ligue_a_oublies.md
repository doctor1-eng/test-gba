# Brief autonome — « Ceux que la Ligue a oubliés »

Prompt de décision rédigé par l'agent pour lui-même (2026-09-26), à la demande de
l'utilisateur : « décide de toi-même ce qui serait le mieux, crée les événements en pixel
art intégrés au jeu, surprends-moi au maximum, sois autonome, livre la ROM finale
seulement ». Il sert aussi de modèle réutilisable pour de futurs lots de contenu.

---

## Rôle

Tu es directeur créatif et lead développeur du fan game Heart & Soul
(pokeemerald-expansion, branche H&S). Tu travailles seul, sans testeur humain, jusqu'à la
livraison d'une ROM finale. Tu décides, tu construis, tu testes toi-même et tu documentes.

## Sources de vérité (dans cet ordre)

1. `docs/heart_and_soul/docs/histoire.md` : ton sombre et réaliste, thème central (Blue
   estime que la Ligue récompense l'image plutôt que la force), réputation (§9),
   épilogues (§10), aspirants (§12).
2. `docs/heart_and_soul/docs/anime-exclusive-locations-audit.md` : seuls les lieux anime
   jugés fiables et thématiquement utiles sont retenus.
3. Le code réel (`data/scripts/heart_and_soul_*.inc`, `include/constants/flags_hns.h`,
   moteur `src/`). Il prime sur toute documentation.

## Critères de décision (filtre obligatoire pour chaque idée)

| Critère | Question |
|---|---|
| Thème | L'idée met-elle en scène le débat de Blue (légitimité, image contre force, oubliés du système) ? |
| Conséquence | Un choix du joueur change-t-il quelque chose de visible (réputation, épilogue, PNJ) ? |
| Continuité | L'idée réutilise-t-elle un fil déjà posé (réfugiés de l'Acte I, Growlithe blessé, Game Corner de Céladopole) ? |
| Surprise | Le joueur verra-t-il quelque chose qu'il ne croyait pas possible dans cette ROM (pixel art inédit, mise en scène, combat spécial) ? |
| Coût | Faisable en réemployant layouts et tilesets existants, sans dépasser la ROM (94,6 %) ? |
| Vérifiable | Peut-on prouver que ça marche en jeu (émulateur headless + captures) ? |

## Décision prise

Deux événements, chacun pleinement abouti, plutôt que cinq ébauches. Ensemble, ils forment
un arc annexe : **« Ceux que la Ligue a oubliés »**.

### Événement 1 — Le Village Caché (anime EP010, « Bulbasaur and the Hidden Village »)

- **Lieu** : une nouvelle carte, cachée au bout d'un sentier de la Route 25 (au nord
  d'Azuria).
- **Thème** : les Pokémon abandonnés pendant l'exode de Cinnabar, ceux dont personne ne
  parle.
- **Continuité** : le texte varie selon les choix de l'Acte I (réfugiés guidés ou cachés,
  Growlithe soigné ou non).
- **Mise en scène** : un Bulbizarre gardien barre la route et combat, Mélanie
  intervient, puis des « collecteurs » Rocket viennent réquisitionner les Pokémon pour
  l'armée de Blue et le joueur affronte un combat double.
- **Choix final** : emmener Bulbizarre (récompense de gameplay) ou le laisser protéger le
  village (+ réputation, objet). Dans les deux cas, le village est mentionné dans
  l'épilogue.

### Événement 2 — Les Arènes Libres (anime « Dark City », arènes non reconnues)

- **Lieu** : un bâtiment sans porte fonctionnelle dans une ruelle de Céladopole, à deux
  pas du Game Corner (façade de financement Rocket, `histoire.md` §8), transformé en arène
  clandestine.
- **Thème** : deux chefs d'arène non reconnus, Kaz et Yas, se disputent la « licence »
  que promet Vesper, recruteuse de Blue. La joueuse est la seule vraie Championne de la
  Ligue dans la pièce.
- **Mise en scène** : l'arrivée pendant une rixe, un double défi, puis un jugement (choix
  à 3 options). Selon ce jugement, combat contre Vesper, ou départ de Kaz et Yas avec
  Blue.
- **Conséquences** : réputation, et une ligne d'épilogue (dans l'épilogue exemplaire, Kaz
  et Yas rejoignent la coda des nouvelles arènes, écho au §12).

### Pixel art

Nouveaux sprites overworld, dérivés de bases existantes du jeu (même palette de rendu,
même gabarit de 9 frames 16×32) pour rester cohérents avec le style GBA :
- Mélanie
- Kaz
- Yas
- Vesper

### Rejeté (et pourquoi)

- **Pokémon Land, concours et Grand Festival** : ton, coût et moteur déjà présent sans
  enjeu narratif.
- **Lieux de Johto et des îles Orange** : hors périmètre.
- **Lieux non vérifiés de l'audit** : risque d'affirmer un canon faux.

## Contraintes techniques non négociables

- Réemploi avant invention : layouts et tilesets existants, gabarits de sprites
  existants.
- Pièges connus (`CONTEXTE_PROJET` §6) : `call` finit par `return` ; `map_script_2` doit
  écrire sa variable sur chaque chemin ; ne rien exécuter après un `warp` dans le même
  script ; forme de `trainerbattle` avec script de victoire ; pas d'apostrophe isolée
  dans un commentaire `@`.
- Chaque flag ou variable nouveau est enregistré dans `flags_hns.h`.
- `make hns` doit passer avec 0 erreur.

## Définition de « terminé »

1. Build propre.
2. Chaque événement joué de bout en bout dans l'émulateur headless
   (`tools/playtest/harness`) avec captures à l'appui : entrée, dialogues, combats, les
   deux branches de chaque choix, sortie, absence de freeze, flags posés.
3. Non-régression : une nouvelle partie démarre et l'Acte I se lance toujours.
4. Documentation : `implementation_notes.md`, `quick_test_checklist.md`, ce brief.
5. Livraison : `pokehns.gba.gz` et un récapitulatif visuel construit à partir de vraies
   captures de la ROM.
