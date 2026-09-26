# Audit — `anime-exclusive-locations-heart-and-soul.md`

Rédigé le 2026-09-26. Source auditée : copie verbatim dans ce même dossier (origine :
OneDrive, *Projet Claude Thomas > Base de connaissance*).

**Verdict : utilisable comme banque d'idées, pas comme spécification.** Le document a
été produit par un LLM sans vérification de sources. Bulbapedia est bloqué par le proxy
réseau de l'environnement de dev, donc les faits ci-dessous sont évalués de mémoire,
avec un niveau de confiance explicite. Tout lieu retenu doit être revérifié sur
Bulbapedia (par l'utilisateur) avant d'écrire des dialogues qui s'en réclament.

## 1. Erreurs factuelles et éléments douteux

| Élément du document | Problème | Confiance |
|---|---|---|
| Parecool Bananaparc (AG088) | Lieu et Pokémon « Parecool » inexistants à ma connaissance, et Hoenn de toute façon. Probable hallucination. | Haute |
| Charicific Valley classée Tier 1 **Kanto**, EP134 | Lieu **Johto** (épisode « Charizard's Burning Ambitions », numéro à revérifier, plutôt ~EP143). | Moyenne-haute |
| Dark City : « Kaz vs A.J. » | Les arènes rivales de Dark City sont **Kaz Gym et Yas Gym**. A.J. vient d'un autre épisode (arène non officielle, 98 victoires). Numéro d'épisode EP042 à vérifier. | Haute (noms) / basse (numéro) |
| Gringey City : « nettoyage Silph Co local » | L'épisode (« Sparks Fly for Magnemite ») parle de pollution et d'une **centrale électrique**, pas de Silph. | Moyenne-haute |
| Kanto Grand Festival : hosts « Lilian Meridian + Jessie/Jessadiah » | Lilian présente Hoenn. Kanto : **Vivian Meridian**. « Jessadiah » n'existe pas (le déguisement de Jessie en coordinatrice est Jessilina). | Moyenne-haute |
| Celadon « basée sur Kyoto » | Faux. C'est **Rosalia/Ecruteak** (Johto) qui s'inspire de Kyoto. | Haute |
| Orange Islands : « Unofficial Champion tournament (Pummelo) » | La Ligue Orange est une ligue **officielle** (Supreme Gym Leader Drake). | Haute |
| Contests « présents dans chaque ville » | Exagération. L'anime en montre beaucoup, pas partout. | Moyenne |
| Xanadu Nursery, Mt. Hideaway (+ Bruno), Neon Town, Lake Slowpoke, Snowtop Mountain, Eggsester, Bloomingvale, Dragon Holy Land, liste Tier 3 entière | Noms et/ou épisodes que je ne peux pas confirmer. Certains ressemblent à des inventions ou à des noms d'autres régions (Gardenia = Championne de Sinnoh). | Basse — **à vérifier avant tout usage** |
| « Pokémon Tech » en Phase 1 | Réel (EP009), mais absent des tableaux du document : incohérence interne. | Haute |
| Chiffres « 70+ / 50+ / 30+ zones » | Non sourcés. | — |

Éléments **fiables** (confiance haute) : Hidden Village + Melanie (EP010), Bill's
Lighthouse (EP013), Pokémon Land (EP017), Pokémon Tech (EP009), Alto Mare (film 5),
Arborville (film 4), règles générales du Grand Festival (5 rubans, tour préliminaire
à un seul appel, combats chronométrés 5 min, Ruban Cup, jury Contesta/Sukizo/Joy).

## 2. Hypothèses implicites du document qui ne tiennent pas pour H&S

1. **« Contests à implémenter »** : faux dans ce fork. Le moteur de concours
   pokeemerald (5 catégories, rangs Normal→Master, rubans, IA) est déjà présent
   (`src/contest.c`, `ContestHall*_hns`), et **Jadielle (`ViridianCity_hns`, warp
   `(33,27)`) mène déjà à un hall de concours** (`MAP_LILYCOVE_CITY_CONTEST_LOBBY_HNS`).
   Le travail restant est de l'habillage (textes, PNJ), pas du moteur.
2. **« Phase 1 = Routes 1-5, early game »** : H&S démarre à Cinnabar avec une
   Championne niveau 34 dans un Kanto ouvert. Il n'y a pas de progression par
   routes à la FRLG. Le phasage doit se caler sur les **Actes I-V**, pas sur les
   numéros de route.
3. **Johto et îles Orange** : hors périmètre. Johto est la trame du jeu de base HnS,
   et H&S est une refonte de Kanto.
4. **Coût ignoré** : ROM à **94,56 %** (build du 2026-09-26, ~1,8 Mo libres). Chaque
   zone qui demande un nouveau tileset est chère. Il faut créer les lieux avec des
   tilesets existants (principe « réemploi avant invention »).
5. **Ton** : Pokémon Land, mini-jeux et « Bananaparc » vont à l'encontre du ton
   « sombre et réaliste » exigé par `histoire.md` §1.

## 3. Ce qui sert vraiment l'histoire (filtre : thème de Blue)

Le conflit central de H&S (`histoire.md` §3) : Blue estime que la Ligue récompense
**l'image et les connexions plutôt que la force réelle**. Les lieux anime qui
*mettent en scène ce débat* valent plus que des zones de plus.

| Lieu | Lien narratif H&S | Ancrage proposé | Coût | Recommandation |
|---|---|---|---|---|
| **Dark City** (arènes non reconnues qui se battent pour la certification de la Ligue) | Miroir exact du grief de Blue : légitimité accordée par la Ligue, pas gagnée. Recrutement Rocket crédible. | Quartier/annexe d'une ville existante (Céladopole ou Safrania), tileset urbain existant | Moyen | **Priorité 1** |
| **Refuge type Hidden Village** (Pokémon abandonnés, gardienne type Melanie) | Prolonge les choix de l'Acte I (réfugiés, Growlithe blessé) : conséquence visible du choix. | Route 25 ou forêt existante, PNJ + drapeaux existants | Faible | **Priorité 1** |
| **Gringey City** (pollution, centrale) | Colle à Terrence / Route de la Centrale (Acte III) : sabotage Rocket, enjeu civil. | Habillage de la Centrale et de ses abords, pas une nouvelle ville | Faible-moyen | **Priorité 2** |
| **Pokémon Tech** (école élitiste pour enfants riches) | Incarne « les connexions contre la force », argument pour et contre Blue. | Bâtiment intérieur réemployé (Jadielle ou Céladopole) | Faible | **Priorité 2** |
| **Concours + Grand Festival** | Le jury qui note l'apparence est l'antithèse de la méritocratie de Blue : terrain idéal pour le faire douter ou le conforter. | Hall déjà câblé à Jadielle. Grand Festival = post-game optionnel. | Faible (hall) / élevé (festival) | Hall : **Priorité 3**. Festival : **reporter** |
| Bill's Lighthouse | Quasi identique à la Maison de Bill (Route 25) déjà en jeu | Dialogues seulement | Très faible | Optionnel |
| Pokémon Land, Charicific Valley, lieux Johto/Orange, Tier 3 | Aucun lien avec l'intrigue, ou hors région, ou non vérifiés | — | — | **Écarter** |

## 4. Prochaines étapes proposées

1. L'utilisateur valide ou amende la shortlist ci-dessus (5 lieux maximum).
2. Pour chaque lieu retenu : vérification Bulbapedia (manuelle), puis fiche d'une
   page dans `histoire.md` §8 (histoires secondaires) avec acte, flags et
   conséquences sur la réputation (§9).
3. Implémentation lieu par lieu, en réemployant les maps et tilesets existants.
   `make hns` doit passer, avec une entrée dans `implementation_notes.md` à chaque lieu.
4. Question ouverte à trancher : le hall de concours de Jadielle reste-t-il
   accessible dès le début, ou est-il fermé par la crise Rocket et rouvert en
   post-game ?
