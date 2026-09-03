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
- [ ] Sac : 5 Poké Balls présentes, + les 8 CT de capacités de terrain (Coupe, Vol, Surf, Force, Flash, Rock Smash, Cascade, Tourbillon), + les 3 cannes à pêche (Old/Good/Super Rod).
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
- [ ] Mart : parler au vendeur (comptoir) → un vrai magasin s'ouvre avec Poké Ball/Potion/Antidote/Anti-Paralysie à acheter.
- [ ] Arène : Blue est absent, le PNJ à l'intérieur explique que l'Arène est fermée (pas de combat possible).

## 6. Forêt de Jade (= « Viridian Forest » affiché en anglais à l'écran — c'est la même zone, juste après Viridian/Argenta, avant Pewter/Jadielle)

- [ ] Combattre Doug (Bug Catcher) — il erre (déplacement aléatoire) **assez loin de l'entrée nord**, plutôt vers le centre-sud de la forêt, proche de la sortie côté Jadielle/Pewter. Pas de flag caché : toujours présent, juste facile à rater dans une map en labyrinthe. Continuer vers le sud si non trouvé près de l'entrée.
- [ ] **Juste après la victoire**, sans avoir à lui reparler : dialogue bûcherons déplacés, choix aider/ignorer fonctionnent.

## 7. Argenta → Jadielle → Azuria (route et niveaux)

- [ ] Pokémon sauvages rencontrés sur Route 1/2/3/4, Forêt de Jade, Mont Sélénite : niveau ~34 (pas 45+).
- [ ] Dresseurs rencontrés sur la route (Danny, Ed, Rob, etc.) : niveau ~34.
- [ ] Coupe/Surf/Vol/Force/etc. utilisable **sans badge** depuis le menu Pokémon (CT + Pokémon compatible fournis dès le départ, voir section 2).

## 8. Arène de Pierre (Jadielle)

- [ ] Mart de Jadielle : le vendeur au comptoir vend Poké Ball/Potion/Antidote/Anti-Paralysie/Réveil/Anti-Brûlure/Corde Sortie/Repousse.
- [ ] En arrivant à l'Arène : **seul** notre dialogue (Pierre doute) s'affiche, puis le combat démarre directement — **plus de texte anglais générique** avant le combat.
- [ ] **Perdre volontairement** ce premier combat, puis reparler à Pierre : la scène de doute (en français) doit **se rejouer**.
- [ ] Gagner le combat : badge + CT Éboulement reçus normalement, **puis** un nouveau texte (en français, lié à notre histoire) où Pierre reconnaît avoir été convaincu.

## 9. Azuria

- [ ] Mart d'Azuria : le vendeur vend en plus une Super Potion.
- [ ] **Avant** de réparer les canalisations : à l'Arène, Ondine explique le problème (Arène inondée/bloquée) et dit qu'elle n'écoutera qu'une fois les canalisations réparées — **pas de combat possible** à ce stade.
- [ ] Toujours avant réparation : un PNJ Team Rocket est présent et **interactif** dans l'Arène (parler à lui affiche une ligne de sabotage).
- [ ] PNJ « canalisations sabotées » (Boy, en ville) : choix aider/ignorer fonctionnent, réparation obtenue.
- [ ] **Après** réparation des canalisations : retourner à l'Arène — le PNJ Rocket a disparu, le combat contre Ondine est maintenant possible (notre dialogue, puis le combat) ; en cas de défaite, la scène se rejoue à la tentative suivante.

## 10. Route vers Carmin-sur-Mer (Vermilion)

- [ ] Depuis Azuria, prendre la route vers le nord (Route 5) → traverser Safrania (Saffron, simple passage, pas d'arène à faire ici) → Route 6 → Carmin-sur-Mer (Vermilion). Aucun garde ne doit bloquer ce trajet.
- [ ] Dresseurs et Pokémon sauvages sur ce trajet : niveau ~34.
- [ ] À Carmin-sur-Mer : parler au PNJ « Nerd » (interception radio) → choix aider/ignorer fonctionnent.
- [ ] Mart de Carmin-sur-Mer : le vendeur vend Poké Ball/Super Potion/Antidote/Anti-Paralysie/Réveil/Anti-Gel/Repousse.

## 11. Arène de Major Bob (Carmin-sur-Mer, 4e arène)

- [ ] Même schéma que Pierre/Ondine : notre dialogue (Major Bob doute), puis l'anglais, puis le combat.
- [ ] Perdre volontairement puis retenter : la scène de doute se rejoue.
- [ ] Gagner : badge reçu normalement.

## 12. Portes de Cinnabar (Arène/Manoir/Labo) — build séparé

- [ ] Ce test ne se fait **pas** dans la partie normale : lancer `make hns MAPTEST=1` (voir `MAP_TEST_README.md` à la racine du dépôt) pour une ROM dédiée qui démarre directement à côté des 3 portes sur `CinnabarIsland_hns`.
- [ ] Les 3 portes (Arène, Manoir, Labo) sont visibles et franchissables, chacune mène au bon intérieur.
- [ ] Sortir de chaque bâtiment ramène bien sur Cinnabar (pas sur l'ancien Kanto inerte).
- [ ] **Ne pas entrer dans le Centre Pokémon** dans ce build de test : ça déclenche le script d'attaque de l'Acte I, hors périmètre de ce test précis.
- [ ] **Nouveau** : dans l'Arène, le PNJ ouvrier est présent et son dialogue s'affiche (reconstruction en cours / Blaine au Dojo).
- [ ] **Nouveau** : dans le Labo, la PNJ assistante est présente et son dialogue s'affiche (recherches au ralenti).
- [ ] **Nouveau** : dans le Manoir, les 2 panneaux "carnet de Blaine" sont trouvables et affichent chacun un texte différent.

## 13. Acte III — les 3 lieutenants (partie normale, `pokehns.gba`)

- [ ] **Lyre (Forêt de Jade / Viridian Forest)** : en traversant la forêt (entre Argenta et Jadielle), une dresseuse LYRE (sprite TEAM ROCKET) approche automatiquement et lance un combat. Dialogue d'avant-combat, de défaite (si vous perdez) et d'après-victoire s'affichent correctement.
- [ ] **Selen (Mont Sélénite / Mt Moon)** : au milieu du groupe de Clefairy sur `MtMoon_Outside_hns`, une dresseuse SELEN approche automatiquement et lance un combat. Mêmes vérifications (avant/défaite/après).
- [ ] **Terrence (Route de la Centrale / Rock Tunnel 1F)** : PNJ TERRENCE immobile, **à aborder** (pas d'approche automatique — lui parler avec A). Dérouler la séquence de 3 questions à choix multiples :
  - [ ] Répondre B/A/A (le plus convaincant à chaque fois) → devrait mener à « convaincu », **pas de combat**, Terrence rejoint votre cause.
  - [ ] Réessayer sur une autre partie/sauvegarde en répondant C à la question 1 (agressif) → devrait court-circuiter directement vers un combat complet, sans poser les questions 2 et 3.
  - [ ] Réessayer en visant un score intermédiaire (par exemple B/B/A) → devrait mener à un combat **allégé** (équipe réduite à 2 Pokémon, sans objets tenus) plutôt que l'équipe complète à 4.
  - [ ] Après une défaite (quelle que soit la branche combat), reparler à Terrence : la séquence de questions ne doit **pas** se rejouer, le jeu doit relancer directement le même combat.
- [ ] Pour les 3 : vérifier qu'aucun des 3 lieutenants ne bloque le passage ou ne casse le déplacement après le combat (pas de blocage de contrôles).

## 14. Acte IV — 3 Championnes ralliées (Céladopole/Safrania/Fuchsia)

- [ ] **Erika (Céladopole)** : avant le combat, un dialogue de doute en français s'affiche (méfiance envers une Championne qui a perdu son île), puis le dialogue anglais d'origine, puis le combat. Équipe niveau ~34 (pas 60+).
- [ ] **Sabrina (Safrania)** : même schéma (doute FR → dialogue original → combat), équipe niveau ~34.
- [ ] **Janine (Fuchsia)** : même schéma, son dialogue de doute mentionne son père Koga. Équipe niveau ~34.
- [ ] Pour les 3 : les dresseurs élèves rencontrés avant la Championne (ex. Michelle/Tanya/Julia à Céladopole, Rebecca/Franklin/Doris/Jared à Safrania, Alice/Linda/Cindy/Barry à Fuchsia) sont aussi niveau ~34, pas surclassés.
- [ ] Badge reçu normalement après chaque victoire.

## 15. Acte IV (suite) — 7e Arène : retrouvailles avec Blaine (Seafoam Islands)

- [ ] Avant le combat, une scène de retrouvailles en français s'affiche (Blaine reconnaît le joueur, explique avoir été emmené ici après l'attaque de Cinnabar), puis le dialogue anglais d'origine, puis le combat.
- [ ] Équipe de Blaine niveau ~34 (pas 65+).
- [ ] Badge reçu normalement après victoire.
- [ ] En reparlant à Blaine après la victoire (ou en revenant plus tard) : le texte anglais d'origine s'affiche, suivi d'une ligne française (« On rentre à Cinnabar dès que possible »).

## 16. Fin du scénario — Kess, Mira Voss, Blue, épilogue

- [ ] **Kess (Zone Safari, grotte de Fuchsia)** : à aborder (pas d'approche automatique). Même séquence de conviction à 3 questions que Terrence : convaincue sans combat (score élevé), combat allégé (2 Pokémon), ou combat complet (4 Pokémon) selon les réponses. Après une défaite, reparler à Kess relance directement le même combat sans reposer les questions.
- [ ] **Mira Voss (Silph Co, Safrania)** : combat direct après un court dialogue (pas de choix), équipe niveau ~34.
- [ ] Une fois Erika + Sabrina + Janine + Major Bob + Blaine + Mira Voss tous obtenus/vaincus : le retour à Cinnabar (Route 20/21) n'est plus bloqué, et Blue apparaît désormais à l'Arène de Viridian (auparavant caché).
- [ ] **Blue (Arène de Viridian, 8e badge)** : dialogue de révélation, puis 3 questions à choix multiples, puis le dialogue anglais d'origine, puis le combat. Perdre puis retenter doit rejouer toute la séquence de dialogue (comme Pierre/Ondine/etc., pas comme Terrence/Kess).
- [ ] Juste après la victoire sur Blue : un épilogue en français s'affiche automatiquement (3 variantes possibles selon les choix faits pendant la partie — réputation ternie/équilibrée/exemplaire). Vérifier qu'il ne se rejoue pas en revenant à l'Arène de Viridian plus tard.
- [ ] Si l'épilogue "exemplaire" s'affiche : vérifier que le texte mentionne un accès aux sous-sols du Pokémon Mansion (le contenu réel de cette zone n'est pas encore implémenté — normal que rien de plus ne se passe pour l'instant).

## 17. Les 8 aspirants Champions (rencontres aléatoires)

- [ ] Sur `Route21_hns`, `ViridianForest_hns`, `Route15_hns`, `SeafoamIslands_1F_hns`,
  `Route1_hns`, `RockTunnel_1F_hns` et `CeladonCity_hns` : en entrant/traversant la zone
  plusieurs fois de suite (1 chance sur 20 par visite, donc parfois plusieurs essais
  nécessaires), un dresseur aspirant apparaît parfois là où il n'y avait rien avant.
- [ ] Chaque aspirant a un dialogue d'avant-combat, de défaite et d'après-victoire propre (pas
  de texte générique/dupliqué entre deux aspirants).
- [ ] **Corvin (Route 1)** : ne doit apparaître que **de nuit** (vérifier avec l'horloge de la
  console/`gettimeofday` — ne pas le chercher en plein jour).
- [ ] **Ael (Route 21, ×2)** et **Orin (Route 21, ×2)** : les deux peuvent apparaître sur la
  même route, à des occasions différentes, sans se remplacer l'un l'autre.
- [ ] Chaque aspirant reste rejouable à une rencontre aléatoire ultérieure (pas de flag qui le
  ferait disparaître définitivement après une victoire — comportement voulu, documenté comme
  simplification : pas de vraie boucle « VS Seeker » progressive).
- [ ] En battant au moins 6 des 8 aspirants sur la partie, puis en terminant le scénario
  (voir section 16) avec l'épilogue **exemplaire** : un texte additionnel dédié aux aspirants
  doit s'afficher à la suite de l'épilogue normal.

## 18. Les 18 événements narratifs supplémentaires (section 11)

- [ ] **Acte I** : au fil des scènes déjà connues de la chute de Cinnabar, 4 nouveaux moments
  s'intercalent (choix de sauvetage, carnet du grunt trouvé, choix pour un Pokémon blessé,
  scène avec un marin sur la Route 21 pendant la tempête) — aucun ne doit bloquer la
  progression normale de l'Acte I.
- [ ] **Acte II** : à Jadielle, le jeune dresseur croisé près du Bourg Palette a une ligne de
  dialogue supplémentaire ; à Argenta, le commerçant a une ligne après le discours de Blue
  entendu à la radio/en ville.
- [ ] **Pewter (panneau photo)** : sur `PewterCity_hns`, près du marché, un panneau invisible
  (marcher dessus + A) affiche un vieux cliché de Blaine jeune avec une dresseuse qui
  ressemble au joueur — uniquement visible après l'Acte I (`FLAG_ACTE_1_TERMINE`).
- [ ] **Route 3 (patrouille de reconnaissance)** : entre Jadielle et le Mont Sélénite, un
  Grunt Rocket isolé lance un combat mineur, avant les vrais lieutenants de l'Acte III.
- [ ] **Mont Sélénite** : fragments de folklore Clefairy déjà présents dans le jeu de base,
  visibles avant/pendant la scène avec Selen (pas de nouveau contenu à vérifier ici au-delà de
  ce qui existe déjà).
- [ ] **Capture d'un Champion allié** : au fil de l'Acte III, une ligne de texte évoque la
  capture brève d'un Champion déjà rallié (Pierre ou Ondine) par la Team Rocket.
- [ ] **Terrence (éboulement du Rock Tunnel)** : juste avant la séquence de conviction de
  Terrence, un texte d'éboulement/alliance temporaire s'affiche avant les 3 questions
  habituelles.
- [ ] **Kess (labo caché de la Zone Safari)** : juste avant la séquence de conviction de Kess,
  un texte décrit d'anciennes installations d'expérimentation portant le sceau de Giovanni.
- [ ] **Céladopole (subordonné admiratif)** : un dresseur optionnel, convaincu par le discours
  de Blue mais pas violent, propose un combat facultatif ; sa défaite montre un doute dans son
  texte d'après-combat.
- [ ] **Janine (scission du clan)** : juste après avoir obtenu le badge de Fuchsia, un texte
  supplémentaire de Janine évoque une tentative de sabotage isolée par une minorité de son
  clan, déjà réglée — ne doit apparaître qu'une seule fois.
- [ ] **Silph Co (piste de Blaine + alarme)** : juste avant le combat contre Mira Voss, deux
  textes s'affichent dans l'ordre — un registre de détention au nom de Blaine (précisant qu'il
  est déjà retrouvé), puis une alarme qui se déclenche dans les étages supérieurs.
- [ ] **Retrouvailles avec Blaine (Seafoam)** : déjà couvert par la section 15 de cette
  checklist — pas de nouveau point à tester ici (c'est l'implémentation existante de
  l'événement 17 du scénario).
- [ ] **Retour des alliés (avant Blue)** : juste avant la scène de révélation de Blue à
  l'Arène de Viridian, un texte récapitule les Champions déjà ralliés qui sécurisent l'accès —
  le texte varie légèrement si Terrence et/ou Kess ont été convaincus (pas seulement vaincus)
  au cours de la partie. Ne doit s'afficher qu'une seule fois, même en cas de défaite et de
  nouvelle tentative contre Blue.

---

**Après chaque test**, dites-moi simplement : ce qui a marché (pas besoin de détail), et pour
ce qui a échoué, l'étape exacte de cette liste + ce qui s'est affiché ou pas affiché à
l'écran. C'est ce niveau de détail (comme pour Quinn/Pierre) qui permet de retrouver la vraie
cause au lieu de deviner.
