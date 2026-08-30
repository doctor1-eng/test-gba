# Heart & Soul — Checklist de test rapide

À dérouler dans l'ordre à **chaque nouvelle ROM reçue**, sur une **partie neuve** (jamais une
sauvegarde continuée — plusieurs bugs remontés jusqu'ici venaient de tester sur une
sauvegarde antérieure à l'ajout du contenu testé). Cocher au fur et à mesure ; dès qu'un point
échoue, noter à quelle étape exactement et ce qui s'est passé (texte affiché, blocage, etc.).

## 1. Lancement / création de personnage

- [ ] Écran-titre → Nouvelle Partie → l'écran de choix du genre s'affiche **directement**, sans discours du Professeur Chen.
- [ ] Choix du genre, saisie du nom : fonctionnent normalement.
- [ ] Le jeu démarre au Centre Pokémon de Cinnabar (pas à New Bark Town).

## 2. Choix du type et de l'équipe

- [ ] Le menu des 18 types s'affiche (liste déroulante).
- [ ] Choisir un type **différent de la dernière fois** si possible, pour couvrir les 18 au fil des tests.
- [ ] Sélection de 4 Pokémon distincts : un doublon fait bien rouvrir la liste.
- [ ] Pour au moins un des 4, choix des 4 attaques via le Tuteur de Capacités (bouton B pour arrêter).
- [ ] Sac : 5 Poké Balls présentes, + les 8 CT de capacités de terrain (Coupe, Vol, Surf, Force, Flash, Rock Smash, Cascade, Tourbillon).
- [ ] Sac : la Bicyclette est présente et utilisable.
- [ ] Menu START : entrée « Pokémon » présente (équipe consultable).
- [ ] Menu START : entrée « Pokénav »/Pokégear présente (carte accessible).
- [ ] Chaussures de course actives (bouton B pour courir sur la carte).

## 3. Attaque de Cinnabar (Acte I)

- [ ] Narration de l'attaque, Blaine disparaît.
- [ ] Choix réfugiés (guider / cacher) : les deux options fonctionnent, message de résultat cohérent.
- [ ] Message de fuite → fondu au noir → **téléportation automatique à Pallet Town**, sans avoir à marcher ni utiliser un CS Fly.
- [ ] Essayer de retourner vers Cinnabar (Route 21, vers le sud) : le jeu doit repousser le joueur avec un message, pas de possibilité d'entrer.

## 4. Route 1

- [ ] Combattre Quinn (Cooltrainer F).
- [ ] **Juste après la victoire**, sans avoir à lui reparler : dialogue de post-combat puis proposition de rendre le bracelet trouvé à Cinnabar.
- [ ] Tester les deux choix (rendre / garder) sur des essais différents si possible.

## 5. Viridian (Argenta)

- [ ] Mart : parler au PNJ (ex-« Cooltrainer M ») → proposition de rejoindre la résistance, choix aider/décliner fonctionnent.
- [ ] Arène : Blue est absent, le PNJ à l'intérieur explique que l'Arène est fermée (pas de combat possible).

## 6. Forêt de Jade (= « Viridian Forest » affiché en anglais à l'écran — c'est la même zone, juste après Viridian/Argenta, avant Pewter/Jadielle)

- [ ] Combattre Doug (Bug Catcher).
- [ ] **Juste après la victoire**, sans avoir à lui reparler : dialogue bûcherons déplacés, choix aider/ignorer fonctionnent.

## 7. Argenta → Jadielle → Azuria (route et niveaux)

- [ ] Pokémon sauvages rencontrés sur Route 1/2/3/4, Forêt de Jade, Mont Sélénite : niveau ~34 (pas 45+).
- [ ] Dresseurs rencontrés sur la route (Danny, Ed, Rob, etc.) : niveau ~34.
- [ ] Coupe/Surf/Vol/Force/etc. utilisable **sans badge** depuis le menu Pokémon (CT + Pokémon compatible fournis dès le départ, voir section 2).

## 8. Arène de Pierre (Jadielle)

- [ ] En arrivant : notre dialogue (Pierre doute) s'affiche, **puis** le dialogue anglais d'origine, **puis** le combat démarre.
- [ ] **Perdre volontairement** ce premier combat, puis reparler à Pierre : la scène de doute (en français) doit **se rejouer**, pas seulement l'anglais.
- [ ] Gagner le combat : badge reçu normalement.

## 9. Azuria

- [ ] PNJ « canalisations sabotées » (Boy) : choix aider/ignorer fonctionnent.
- [ ] Arène d'Ondine : même test que Pierre — notre dialogue puis l'anglais puis le combat ; en cas de défaite, la scène de doute se rejoue à la tentative suivante.

## 10. Route vers Carmin-sur-Mer (Vermilion)

- [ ] Depuis Azuria, prendre la route vers le nord (Route 5) → traverser Safrania (Saffron, simple passage, pas d'arène à faire ici) → Route 6 → Carmin-sur-Mer (Vermilion). Aucun garde ne doit bloquer ce trajet.
- [ ] Dresseurs et Pokémon sauvages sur ce trajet : niveau ~34.
- [ ] À Carmin-sur-Mer : parler au PNJ « Nerd » (interception radio) → choix aider/ignorer fonctionnent.

## 11. Arène de Major Bob (Carmin-sur-Mer, 4e arène)

- [ ] Même schéma que Pierre/Ondine : notre dialogue (Major Bob doute), puis l'anglais, puis le combat.
- [ ] Perdre volontairement puis retenter : la scène de doute se rejoue.
- [ ] Gagner : badge reçu normalement.

---

**Après chaque test**, dites-moi simplement : ce qui a marché (pas besoin de détail), et pour
ce qui a échoué, l'étape exacte de cette liste + ce qui s'est affiché ou pas affiché à
l'écran. C'est ce niveau de détail (comme pour Quinn/Pierre) qui permet de retrouver la vraie
cause au lieu de deviner.
