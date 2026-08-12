# CHARACTERS.md

## Joueur (Sacha-équivalent)
- Base moteur : `{PLAYER}` (customisable par le joueur, nom libre comme dans tout Pokémon)
- Starter : Pikachu, remis par le Professeur Chen (pas de choix parmi 3)
- Ton : impulsif, courageux, un peu tête en l'air — laisser transparaître dans les choix de dialogue optionnels

## Professeur Chen (ex-Birch)
- Rôle : mentor, point de départ de l'aventure
- Ton : chaleureux, passionné, un peu bavard — **fait** (`data/text/birch_speech.inc` traduit, Session 1)
- Voix distincte : phrases longues, vocabulaire scientifique accessible, jamais familier

## Pikachu
- Statut : starter + personnage à part entière (cf. GAME_DESIGN_DOCUMENT.md, "Pikachu-personnage")
- Caractérisation : réticent au début, ne veut pas entrer en Poké Ball, tsundere-attitude qui s'adoucit avec la progression
- Implémentation prévue : flags de script pour réactions contextuelles (Phase 2+)

## Régis (rival)
- Rôle : rival du joueur, petit-fils/neveu du Professeur Chen (cohérent avec Bourg Palette)
- Ton : sûr de lui, compétitif, mais jamais malveillant — évolue vers plus de respect envers le joueur
- Équipe : évolutive à chaque rencontre, cohérente avec la progression du joueur (ne pas être artificiellement surpuissant, cf. cahier des charges §17)

## Ondine — Arc 3 complet (Route 2 + championne d'arène d'Azuria)
- Ton établi : franche, un peu autoritaire, passionnée par les Pokémon Eau — premier contact abrasif
  (Pikachu perturbe sa pêche), pas de coup de foudre immédiat, elle teste le joueur en combat avant de
  lui accorder un minimum de respect
- Statut : rivale récurrente (pas de suivi à l'écran comme Pikachu) — recroisée ponctuellement dans les
  arcs suivants
- Rôle : championne d'arène d'Azuria (2e badge, ex-Roxanne/Rustboro). Équipe Eau (Poliwag, Goldeen,
  Staryu), badge renommé BADGE CASCADE (nom canon). Son intro à l'arène référence explicitement leur
  affrontement de la Route 2 ("On se retrouve, {PLAYER}…")

## Pierre (à venir — Arc 4)
- Ton distinct à définir : calme, posé, protecteur, passionné de minéralogie

## Jessie / James / Miaouss — Team Rocket (à venir)
- Ton distinct à définir : comique, grandiloquent, jamais réellement menaçant — schéma "plan absurde → échec comique"
- Voir GAME_DESIGN_DOCUMENT.md pour le principe de récurrence narrative

## Giovanni (à venir)
- Ton distinct à définir : froid, autoritaire, en contraste total avec Jessie/James/Miaouss

---
*Règle de cohérence des voix (cahier des charges §15) : chaque personnage doit être reconnaissable au style
seul, sans tag de nom. Je vérifierai ce critère avant validation finale de chaque arc.*
