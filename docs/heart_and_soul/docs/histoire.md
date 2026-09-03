# Heart & Soul — Histoire complète (Kanto, fork pokeemerald-expansion)

## 1. Prémisse et enjeu central

Le joueur est le Champion d'Arène de **Cinnabar**. Une nuit, la **Team Rocket**, réorganisée et dirigée par **Blue**, envahit l'île et prend le contrôle de la ville et du Pokémon Center. Le mentor du joueur, **Blaine** (ancien Champion, figure tutélaire), disparaît pendant l'attaque — capturé ou en fuite, on l'ignore. La Team Rocket coupe les liaisons maritimes et aériennes : Cinnabar est isolée du reste de Kanto pendant plusieurs jours.

Le joueur s'échappe de justesse avec 4 Pokémon (niveau 34, du type choisi en début de partie) et doit :
- comprendre pourquoi Blue a pris la tête de la Team Rocket et ce qu'il cherche réellement à Cinnabar (le Pokémon Mansion, canoniquement ancien labo Rocket, est le point de départ logique) ;
- reconstruire sa crédibilité auprès des autres Champions, qui doutent de sa capacité à protéger quoi que ce soit ;
- reprendre le contrôle du territoire, zone par zone, avant d'affronter Blue.

**Ton** : sombre et réaliste. Les habitants de Cinnabar sont déplacés, certains blessés, un climat de méfiance s'installe. Aucun happy end facile — chaque victoire a un coût (zones endommagées, PNJ qui ne reviennent pas, confiance à reconstruire).

## 2. Découpage en 5 actes

**Acte I — La chute de Cinnabar**
Zones : Cinnabar, mer environnante (bloquée), Pokémon Mansion (premier contact avec la Team Rocket).
Objectif : survivre à l'attaque, s'échapper de l'île, comprendre que Blaine a disparu.

**Acte II — L'exil et la méfiance**
Zones : Route 1, Viridian, Pewter, Cerulean.
Objectif : rejoindre le continent, convaincre les premiers Champions (Pierre, Ondine) de l'ampleur de la menace. Ils doutent : "tu n'as pas su protéger ta propre ville."

**Acte III — L'ampleur du réseau**
Zones : Viridian Forest, Mont Sélénite, Route de la Centrale (Rock Tunnel).
Objectif : découvrir que la Team Rocket contrôle discrètement plusieurs zones stratégiques de Kanto via des lieutenants régionaux. Premiers affrontements contre ces lieutenants (open world : ordre libre).

**Acte IV — La reconquête**
Zones : Zone Safari, Céladopole, Fuchsia, Saffron/Silph Co.
Objectif : reprendre les zones restantes une à une, retrouver la trace de Blaine (retenu à Silph Co ou en transit vers Cinnabar), obtenir l'alliance complète des Champions (Erika, Koga, Sabrina, Major Bob).

**Acte V — Confrontation finale**
Zones : retour à Cinnabar, Pokémon Mansion.
Objectif : libérer Cinnabar, affronter Blue dans le Mansion (écho direct à l'ancien QG Rocket canonique), révéler sa motivation réelle.

## 3. Antagoniste principal et lieutenants

### Blue — motivation et scène de révélation

**Backstory retenue** : Blue a battu l'Élite 4 dans sa jeunesse mais s'est vu refuser le titre de Champion officiel — la Ligue a invoqué un vice de procédure (un combat contesté, ou son âge à l'époque) pour le remplacer par un candidat plus "présentable" politiquement. Depuis, Blue est convaincu que la Ligue ne récompense pas la force réelle mais l'image et les connexions. Il a pris la tête des restes de la Team Rocket non par goût du crime, mais parce que c'était la seule structure avec assez de ressources et d'hommes pour "forcer" un nouveau système : un classement de Champions basé uniquement sur la performance en combat, sans jury ni politique — quitte à débuter par la force.

Cinnabar n'est pas une cible arbitraire : le Pokémon Mansion contient les anciennes archives de recherche Rocket (canon), que Blue veut récupérer pour prouver que la Ligue elle-même a autrefois couvert les expérimentations de Giovanni — de quoi discréditer l'institution publiquement.

**Scène de révélation (Acte V, confrontation finale au Pokémon Mansion)** — trame de dialogue :

> **Blue** : "Tu crois que je fais ça pour le pouvoir ? J'ai battu l'Élite 4 avant mes seize ans. Ils m'ont dit non — pas assez 'présentable'. Toi, on t'a donné un badge et une ville. Moi, une porte fermée."

**Séquence à choix multiples (3 questions, impact direct sur `VAR_REPUTATION`)** — même logique que les lieutenants convertibles, mais ici la conversion n'est pas l'enjeu (le combat final a toujours lieu) : c'est le **regard porté sur Blue** qui compte pour la réputation du joueur.

1. *Réponse à "une porte fermée"* :
   - A. "Tu as peut-être raison sur l'injustice. Mais ça n'explique pas Cinnabar." (empathie sans excuser) (+2)
   - B. "Peu importe ta raison, tu as détruit des vies." (rejet sec) (0)
   - C. "Le système est cassé, tu as bien fait de le prouver comme ça." (validation totale) (-2)

2. *Réponse à l'accusation sur les archives Giovanni/Ligue* :
   - A. "Alors prouve-le publiquement. Pas comme ça." (constructif) (+1)
   - B. "Ça ne te donne pas le droit de faire souffrir Cinnabar." (ferme, pas cruel) (+1)
   - C. "Personne ne te croira de toute façon." (cynique) (-1)

3. *Dernière réplique avant le combat* :
   - A. "Rends-toi. On réglera ça autrement." (+1)
   - B. "Je vais te prouver que la force ne suffit pas." (neutre) (0)
   - C. "Tu n'es qu'un lâche qui cherche des excuses." (insulte) (-1)

**Conversion en score de réputation** : total de la séquence ≥ 3 → **+2** à `VAR_REPUTATION` ; total entre 0 et 2 → **0** ; total négatif → **-2**. Ces valeurs remplacent la ligne "Confrontation finale avec Blue" du barème de la section 9 sans changer les bornes globales du score (max 22 / min -2).

> **Blue** (après le combat, quelle que soit l'issue des choix) : "Ces archives prouvent que la Ligue savait, pour Giovanni. Elle a laissé faire tant que ça restait discret. Je ne détruis rien qui ne soit pas déjà pourri."

### Lieutenants régionaux

| Zone | Nom | Personnalité / motivation | Style de combat |
|---|---|---|---|
| **Viridian Forest** | **Lyre** | Ancienne garde forestière, convaincue que la Team Rocket va "rendre la forêt aux Pokémon" en chassant les promoteurs immobiliers qui menaçaient la zone. Idéaliste sincère, pas cynique — la plus facile à faire douter en dialogue. | Guérilla : Bug/Poison, pièges et embuscades, combats en plusieurs manches courtes. |
| **Mont Sélénite** | **Selen** | Mystique obsédée par les météorites et le folklore Clefairy, voit Blue comme un "élu" révélant une vérité cachée sur Kanto. Fanatisme religieux plus que criminel. | Rock/Psychic/Fairy, combat lent mais avec beaucoup de statuts et d'effets de terrain. |
| **Route de la Centrale (Rock Tunnel)** | **Terrence** | Ex-mineur ruiné par la fermeture du tunnel, payé en argent sonnant — seul lieutenant à ne pas croire en la cause de Blue, uniquement motivé par la dette. Le plus corruptible en dialogue (peut être retourné). | Rock/Ground, mercenaire pragmatique, joue la sécurité (walls, terrain-control). |
| **Zone Safari** | **Warden Kess** | Ancienne responsable de la réserve, contrainte de braconner pour Blue sous la menace envers ses employés restants. Conflit moral fort — bon candidat pour un choix narratif (l'épargner/la convaincre). | Normal/Grass/Water, équipe variée et opportuniste, combats longs d'usure. |
| **Silph Co (Saffron)** | **Dr. Mira Voss** | Ex-ingénieure Silph écartée d'une promotion au profit d'un cadre mieux connecté — miroir direct de la propre histoire de Blue, ce qui explique sa loyauté totale. La plus dangereuse et la plus lucide des cinq. | Psychic/Steel/Electric, combat technique, contrôle de terrain et de statut, dernier vrai obstacle avant Blue. |

### Mécanique de conviction (Terrence & Kess)

Principe : au lieu d'un combat imposé, chaque lieutenant "retournable" propose une **séquence de 3 questions à choix multiples** avant le combat. Chaque réponse rapporte des points de persuasion (`VAR_PERSUASION_<NOM>`). Le total détermine l'issue :

- **≥ 5 points** : le lieutenant se rallie sans combat, donne une information utile pour l'Acte suivant (ex. localisation de Blaine, faille dans la garde de Silph Co).
- **3-4 points** : combat allégé (équipe réduite ou affaiblie), le lieutenant se rend avant la fin et rejoint quand même la cause du joueur.
- **≤ 2 points** : combat complet classique, aucune conversion possible (le joueur peut retenter à une visite ultérieure si le scénario le permet).

**Terrence (Route de la Centrale)** — sensible à l'argent et à la trahison potentielle de Blue.

1. *"Si tu perds ici, Blue effacera ta dette ?"*
   - A. "Blue tient toujours parole." (+0 — il s'en méfie déjà, réponse plate)
   - B. "As-tu déjà vu quelqu'un d'autre payé après un échec ?" (+2 — sème le doute)
   - C. "Je me fiche de ta dette, bats-toi." (-1 — il se braque, passe en combat complet direct)

2. *"Et si je te trouvais un travail légal qui couvre ta dette ?"*
   - A. "Je peux t'obtenir un contrat minier légitime via [Champion allié]." (+2 — offre concrète)
   - B. "Trouve une solution toi-même." (+0 — neutre, il reste méfiant)
   - C. "Ce n'est pas mon problème." (-1)

3. *"Le tunnel que tu gardes va être détruit si Blue perd — tu le sais ?"*
   - A. "Exactement, et ta ville d'origine avec lui." (+2 — touche son attachement au lieu)
   - B. "Ce n'est qu'un tunnel." (-1)
   - C. Ne rien dire, passer au combat. (+0)

**Kess (Zone Safari)** — motivée par la protection de ses employés, pas par conviction personnelle.

1. *"Blue menace vraiment tes employés si tu n'obéis pas ?"*
   - A. "Je peux organiser leur protection dès maintenant avec [Champion allié]." (+2)
   - B. "C'est vraiment grave ?" (+1 — montre de l'empathie sans solution concrète)
   - C. "Ce n'est pas mon problème, bats-toi." (-1)

2. *"Le braconnage que tu fais pour Blue, tu y crois ?"*
   - A. "Non, mais je n'ai pas le choix." (réponse du PNJ — le joueur répond ensuite : "Alors laisse-moi te donner ce choix.") (+2)
   - B. Le joueur menace de révéler son rôle publiquement. (-1 — elle se braque, se sent acculée)
   - C. Le joueur ne répond rien de concret. (+0)

3. *"Si je bats Blue, la réserve retrouve son statut légal — tu as ma parole."*
   - A. Accepter la promesse. (+1 — dépend du score cumulé précédent pour basculer au-dessus du seuil)
   - B. Exiger une preuve immédiate (garantie écrite d'un Champion). (+2 — la plus crédible pour un personnage pragmatique)
   - C. Ignorer et combattre directement. (+0)

**Implémentation technique suggérée** :
- `VAR_PERSUASION_TERRENCE` / `VAR_PERSUASION_KESS`, incrémentées à chaque choix de dialogue.
- À la fin de la séquence, un script vérifie le seuil et redirige vers l'un des 3 scripts d'issue (ralliement direct / combat allégé / combat complet).
- Le choix C "agressif" en question 1 peut court-circuiter directement vers le combat complet sans poser les questions 2 et 3, pour éviter un dialogue qui sonnerait faux après une hostilité affichée.


## 4. Rôle des autres Champions d'Arène

- **Pierre (Pewter)** et **Ondine (Cerulean)** : premiers contactés, sceptiques, se rallient progressivement (Acte II).
- **Major Bob (Vermilion)** : logistique et informations sur les mouvements Rocket.
- **Erika (Celadon), Koga (Fuchsia), Sabrina (Saffron)** : alliés clés de l'Acte IV, chacun lié à une zone à reprendre.
- **Giovanni / Viridian** : gym vacante ou sous influence Rocket historique — écho volontaire au canon (Giovanni était déjà lié à la Team Rocket), sans réutiliser le personnage lui-même.
- **Blaine** : mentor disparu, retrouvé prisonnier en Acte IV/V — fil rouge émotionnel de toute l'histoire.

## 5. Structure open world

- Après l'Acte II, les zones de l'Acte III (Viridian Forest, Mont Sélénite, Rock Tunnel) sont **ouvertes simultanément** : le joueur choisit l'ordre.
- Chaque zone a un **niveau recommandé** (30-40) et un **obstacle narratif** (lieutenant à vaincre, PNJ à convaincre) plutôt qu'un blocage artificiel.
- L'Acte IV s'ouvre entièrement une fois 2 des 3 lieutenants de l'Acte III vaincus (seuil narratif plutôt que linéaire strict).
- Cinnabar reste fermée jusqu'à la fin de l'Acte IV (cohérent avec son isolement géographique).

## 6. Liste des ~20 Pokémon niveau 34 par type

*Hypothèse confirmée : le fork utilise le Pokédex national complet — la liste des 18 types en section 6 est donc utilisable telle quelle, sans retrait.*

| Type | Pokémon proposés (niv. 34) | Répartition des rôles |
|---|---|---|
| Normal | Snorlax, Kangaskhan, Chansey, Blissey, Tauros, Ditto, Slaking, Girafarig, Furret, Linoone, Zangoose, Bibarel, Stoutland, Diggersby, Lopunny, Ursaring, Exploud, Doduo, Dodrio, Porygon2 | Physiques : Snorlax, Kangaskhan, Tauros, Slaking, Ursaring, Zangoose, Diggersby, Lopunny, Exploud, Dodrio · Spéciaux : Porygon2, Girafarig · Supports/Walls : Chansey, Blissey, Ditto, Bibarel, Stoutland, Linoone, Furret, Doduo |
| Feu | Charizard, Arcanine, Rapidash, Magmortar, Flareon, Ninetales, Typhlosion, Torkoal, Camerupt, Houndoom, Infernape, Blaziken, Emboar, Delphox, Talonflame, Volcarona, Chandelure, Simisear, Pyroar, Magcargo | Physiques : Arcanine, Rapidash, Infernape, Blaziken, Emboar, Talonflame · Spéciaux : Charizard, Magmortar, Flareon, Ninetales, Typhlosion, Delphox, Volcarona, Chandelure, Pyroar · Walls/Supports : Torkoal, Camerupt, Houndoom, Simisear, Magcargo |
| Eau | Blastoise, Gyarados, Vaporeon, Poliwrath, Golduck, Starmie, Lapras, Kingdra, Milotic, Feraligatr, Swampert, Empoleon, Greninja, Azumarill, Slowbro, Seaking, Kabutops, Omastar, Whiscash, Ludicolo | Physiques : Gyarados, Poliwrath, Feraligatr, Swampert, Azumarill, Kabutops, Kingdra · Spéciaux : Blastoise, Vaporeon, Starmie, Lapras, Milotic, Empoleon, Greninja · Walls/Supports : Golduck, Slowbro, Seaking, Omastar, Whiscash, Ludicolo |
| Plante | Venusaur, Vileplume, Victreebel, Tangrowth, Exeggutor, Leafeon, Sceptile, Torterra, Serperior, Roserade, Breloom, Cradily, Sunflora, Meganium, Ferrothorn, Lurantis, Trevenant, Whimsicott, Gogoat, Abomasnow | Physiques : Breloom, Torterra, Gogoat, Lurantis, Leafeon · Spéciaux : Venusaur, Vileplume, Victreebel, Sceptile, Serperior, Roserade, Sunflora, Meganium · Walls/Supports : Tangrowth, Exeggutor, Cradily, Ferrothorn, Trevenant, Whimsicott, Abomasnow |
| Électrik | Raichu, Electrode, Electivire, Jolteon, Ampharos, Luxray, Manectric, Magnezone, Zebstrika, Eelektross, Rotom, Galvantula, Heliolisk, Lanturn, Emolga, Vikavolt, Toxtricity, Dedenne, Pikachu, Stunfisk | Physiques : Electivire, Luxray, Zebstrika, Eelektross · Spéciaux : Raichu, Jolteon, Ampharos, Manectric, Magnezone, Rotom, Galvantula, Heliolisk, Toxtricity · Walls/Supports : Electrode, Lanturn, Emolga, Vikavolt, Dedenne, Pikachu, Stunfisk |
| Glace | Glaceon, Weavile, Froslass, Mamoswine, Walrein, Cloyster, Abomasnow, Vanilluxe, Beartic, Avalugg, Cryogonal, Ninetales (Alola), Sandslash (Alola), Jynx, Aurorus, Piloswine, Delibird, Frosmoth, Lapras, Sneasel | Physiques : Weavile, Mamoswine, Beartic, Sandslash (Alola), Sneasel, Piloswine · Spéciaux : Glaceon, Froslass, Vanilluxe, Cryogonal, Ninetales (Alola), Jynx, Aurorus, Frosmoth · Walls/Supports : Walrein, Cloyster, Abomasnow, Avalugg, Delibird, Lapras |
| Combat | Machamp, Hitmonlee, Hitmonchan, Hitmontop, Heracross, Lucario, Toxicroak, Gallade, Conkeldurr, Throh, Sawk, Scrafty, Pangoro, Passimian, Sirfetch'd, Primeape, Poliwrath, Blaziken, Infernape, Emboar | Physiques : Machamp, Hitmonlee, Hitmonchan, Hitmontop, Heracross, Conkeldurr, Throh, Sawk, Pangoro, Passimian, Sirfetch'd, Primeape, Emboar · Spéciaux : Lucario, Gallade, Toxicroak, Infernape · Walls/Supports : Poliwrath, Scrafty, Blaziken |
| Poison | Muk, Weezing, Crobat, Venomoth, Nidoking, Nidoqueen, Drapion, Skuntank, Toxapex, Naganadel, Seviper, Arbok, Beedrill, Tentacruel, Salazzle, Amoonguss, Toxicroak, Vileplume, Victreebel, Gengar | Physiques : Muk, Nidoking, Drapion, Skuntank, Seviper, Arbok, Beedrill, Toxicroak · Spéciaux : Weezing, Venomoth, Nidoqueen, Naganadel, Salazzle, Vileplume, Victreebel, Gengar · Walls/Supports : Crobat, Toxapex, Tentacruel, Amoonguss |
| Sol | Dugtrio, Sandslash, Rhydon, Golem, Marowak, Donphan, Flygon, Garchomp, Excadrill, Krookodile, Gliscor, Camerupt, Torterra, Hippowdon, Claydol, Gastrodon, Stunfisk, Nidoking, Steelix, Whiscash | Physiques : Dugtrio, Sandslash, Rhydon, Golem, Marowak, Donphan, Garchomp, Excadrill, Krookodile, Gliscor, Nidoking · Spéciaux : Flygon, Camerupt, Torterra, Claydol · Walls/Supports : Hippowdon, Gastrodon, Stunfisk, Steelix, Whiscash |
| Vol | Pidgeot, Fearow, Aerodactyl, Charizard, Dodrio, Skarmory, Crobat, Togekiss, Staraptor, Honchkrow, Swellow, Noivern, Talonflame, Braviary, Mandibuzz, Corviknight, Tornadus, Gyarados, Salamence, Altaria | Physiques : Pidgeot, Fearow, Aerodactyl, Dodrio, Staraptor, Honchkrow, Swellow, Talonflame, Braviary, Mandibuzz, Salamence · Spéciaux : Charizard, Togekiss, Noivern, Tornadus, Altaria · Walls/Supports : Skarmory, Crobat, Corviknight, Gyarados |
| Psy | Alakazam, Slowbro, Starmie, Mr. Mime, Exeggutor, Gardevoir, Gallade, Metagross, Reuniclus, Bronzong, Sigilyph, Musharna, Espeon, Xatu, Slowking, Malamar, Oranguru, Girafarig, Wobbuffet, Grumpig | Physiques : Gallade, Metagross, Malamar · Spéciaux : Alakazam, Starmie, Mr. Mime, Exeggutor, Gardevoir, Reuniclus, Sigilyph, Espeon, Slowking, Grumpig · Walls/Supports : Slowbro, Bronzong, Musharna, Xatu, Oranguru, Girafarig, Wobbuffet |
| Insecte | Butterfree, Beedrill, Scizor, Pinsir, Heracross, Venomoth, Volcarona, Galvantula, Vikavolt, Yanmega, Ninjask, Shedinja, Vespiquen, Crustle, Escavalier, Accelgor, Leavanny, Forretress, Armaldo, Volbeat | Physiques : Beedrill, Scizor, Pinsir, Heracross, Ninjask, Escavalier, Leavanny, Armaldo · Spéciaux : Butterfree, Venomoth, Volcarona, Galvantula, Vikavolt, Yanmega, Accelgor · Walls/Supports : Shedinja, Vespiquen, Crustle, Forretress, Volbeat |
| Roche | Golem, Onix, Rhydon, Kabutops, Omastar, Aerodactyl, Tyranitar, Aggron, Rampardos, Bastiodon, Probopass, Gigalith, Carbink, Lycanroc, Stonjourner, Coalossal, Crustle, Cradily, Relicanth, Barbaracle | Physiques : Golem, Rhydon, Kabutops, Aerodactyl, Tyranitar, Rampardos, Gigalith, Lycanroc, Barbaracle · Spéciaux : Omastar, Cradily · Walls/Supports : Onix, Aggron, Bastiodon, Probopass, Carbink, Stonjourner, Coalossal, Crustle, Relicanth |
| Spectre | Gengar, Mismagius, Dusknoir, Drifblim, Chandelure, Trevenant, Aegislash, Sableye, Banette, Golurk, Froslass, Cofagrigus, Polteageist, Runerigus, Gourgeist, Dhelmise, Palossand, Spiritomb, Jellicent, Marowak (Alola) | Physiques : Dusknoir, Golurk, Banette, Runerigus, Dhelmise, Marowak (Alola) · Spéciaux : Gengar, Mismagius, Chandelure, Trevenant, Aegislash, Froslass, Polteageist, Jellicent · Walls/Supports : Drifblim, Sableye, Cofagrigus, Gourgeist, Palossand, Spiritomb |
| Dragon | Dragonite, Kingdra, Salamence, Garchomp, Flygon, Altaria, Haxorus, Hydreigon, Goodra, Noivern, Dragalge, Turtonator, Drampa, Duraludon, Kommo-o, Druddigon, Tyrantrum, Flapple, Appletun, Dragapult | Physiques : Dragonite, Salamence, Garchomp, Haxorus, Druddigon, Tyrantrum, Kommo-o · Spéciaux : Kingdra, Flygon, Altaria, Hydreigon, Goodra, Noivern, Dragalge, Turtonator, Drampa, Dragapult · Walls/Supports : Duraludon, Flapple, Appletun |
| Ténèbres | Tyranitar, Houndoom, Absol, Weavile, Honchkrow, Krookodile, Bisharp, Umbreon, Sableye, Scrafty, Malamar, Mandibuzz, Zoroark, Hydreigon, Drapion, Skuntank, Pangoro, Grimmsnarl, Obstagoon, Incineroar | Physiques : Tyranitar, Absol, Weavile, Krookodile, Bisharp, Drapion, Pangoro, Obstagoon, Incineroar · Spéciaux : Houndoom, Honchkrow, Malamar, Zoroark, Hydreigon, Skuntank, Grimmsnarl · Walls/Supports : Umbreon, Sableye, Scrafty, Mandibuzz |
| Acier | Steelix, Skarmory, Metagross, Aggron, Empoleon, Bisharp, Scizor, Excadrill, Ferrothorn, Klinklang, Bronzong, Probopass, Corviknight, Aegislash, Duraludon, Magnezone, Lucario, Mawile, Copperajah, Forretress | Physiques : Metagross, Bisharp, Scizor, Excadrill, Lucario, Mawile, Copperajah · Spéciaux : Empoleon, Klinklang, Magnezone, Aegislash, Duraludon · Walls/Supports : Steelix, Skarmory, Aggron, Ferrothorn, Bronzong, Probopass, Corviknight, Forretress |
| Fée | Clefable, Togekiss, Gardevoir, Sylveon, Granbull, Mawile, Azumarill, Whimsicott, Florges, Primarina, Ribombee, Mimikyu, Grimmsnarl, Hatterene, Alcremie, Dedenne, Klefki, Slurpuff, Ninetales (Alola), Wigglytuff | Physiques : Granbull, Mawile, Azumarill, Mimikyu, Grimmsnarl · Spéciaux : Togekiss, Gardevoir, Sylveon, Florges, Primarina, Ribombee, Hatterene, Ninetales (Alola) · Walls/Supports : Clefable, Whimsicott, Alcremie, Dedenne, Klefki, Slurpuff, Wigglytuff |

## 7. Format technique (à adapter au fork)

- Chaque acte correspond à un **flag principal** (ex. `FLAG_ACTE_1_TERMINE`) qui conditionne l'accès aux zones suivantes.
- Chaque lieutenant vaincu active un **flag régional** (ex. `FLAG_LIEUTENANT_FORET_VAINCU`), utilisé pour ouvrir l'Acte IV dès que 2 des 3 flags de l'Acte III sont actifs.
- Le choix de type en début de partie fixe une **variable persistante** (`VAR_TYPE_CHOISI`) qui conditionne l'affichage de la liste de 20 Pokémon et certains dialogues (les autres Champions réagissent différemment selon le type du joueur).
- Blaine et les PNJ affectés par l'attaque doivent avoir un **état binaire simple** (sain et sauf / capturé / disparu) piloté par flag, pour rester gérable en scripting.

---

## 8. Histoires secondaires par lieu

**Cinnabar (île, Acte I et V)**
*"Les derniers habitants"* — avant l'évacuation forcée, le joueur croise des habitants cachés dans les grottes du Mont Cinnabar. Choix : les guider vers un bateau de fortune (risque d'être repéré par la Team Rocket) ou les cacher sur place (risque à plus long terme, sans garantie de retour). Le résultat de ce choix influence qui est retrouvé vivant à Cinnabar en Acte V.

**Pokémon Mansion**
*"Les carnets de Blaine"* — des fragments de journal dispersés dans le manoir révèlent progressivement les recherches originales de Giovanni et les soupçons que Blaine nourrissait déjà à l'époque. Ce fil se referme en Acte V : c'est ce que Blue cherchait précisément à récupérer.

**Route 21 (traversée maritime Cinnabar → Pallet)**
Séquence de fuite plutôt que quête annexe : possibilité de secourir un dresseur en perdition en mer, moment qui installe l'isolement et la vulnérabilité du joueur juste après la chute de sa ville.

**Pallet Town**
Ville épargnée, presque indifférente à la chute de Cinnabar au début. Un jeune dresseur local incarne le monde "d'avant" — recroisé en Acte III/IV quand la Team Rocket s'étend jusqu'à sa propre route, illustrant la progression de la menace.

**Route 1**
Point de contrôle improvisé tenu par une milice locale de Viridian, méfiante envers les inconnus depuis les rumeurs sur Cinnabar. Petite quête : retrouver un objet perdu d'une famille de réfugiés pour obtenir le passage sans affrontement.

**Viridian City**
Débat local non tranché : certains habitants pensent que la présence Rocket "apporte de l'ordre" en l'absence de Champion (écho volontaire à Giovanni). Un commerçant finance en secret une résistance locale — piste optionnelle pour obtenir des informations avant l'Acte III.

**Viridian Forest**
Au-delà de Lyre : des bûcherons se retrouvent sans emploi à cause de la politique de "reconquête de la forêt" version Rocket — sous-intrigue qui nuance l'idéalisme de Lyre en montrant ses dégâts collatéraux.

**Pewter City**
Pierre a discrètement stocké des vivres pour les réfugiés de Cinnabar avant même de s'engager publiquement — révélé au joueur seulement s'il gagne sa confiance, renforçant le thème "la confiance se reconquiert, elle ne se décrète pas".

**Mont Sélénite (Mt Moon)**
Au-delà de Selen : une chercheuse rationaliste étudiant les Clefairy et les météorites offre un contrepoint scientifique au fanatisme de Selen, sans pour autant invalider tout le folklore — ambiguïté volontaire.

**Cerulean City**
Le sabotage Rocket des canalisations a provoqué une inondation partielle de la ville, réduisant l'Arène d'Ondine à un poste de secours temporaire. Quête de réparation avant qu'Ondine ne puisse pleinement rejoindre la cause.

**Route de la Centrale (Rock Tunnel)**
Au-delà de Terrence : des mineurs restés piégés dans une galerie secondaire, sauvables indépendamment de l'issue avec Terrence (conviction ou combat) — occasion de nuancer le personnage même si le joueur choisit de le combattre.

**Vermilion City**
Major Bob dirige une interception des transmissions radio Rocket. Petite séquence d'écoute/puzzle donnant des indices sur les mouvements à Silph Co avant l'Acte IV.

**Zone Safari**
Au-delà de Kess : des Pokémon braconnés peuvent être libérés par le joueur, avec un impact visible (mais non chiffré en jeu) sur la réputation du joueur auprès des gardes de la réserve.

**Fuchsia City**
Le clan ninja de Koga est divisé entre engagement direct contre la Team Rocket et stratégie de l'ombre. Le choix du joueur dans ce débat conditionne quels alliés sont disponibles pour l'assaut final.

**Céladopole (Celadon City)**
Le grand magasin sert de façade de financement pour la Team Rocket (écho volontaire au Game Corner canonique). Quête d'infiltration pour exposer ce financement avant qu'Erika ne s'engage pleinement.

**Saffron City / Silph Co**
Avant la confrontation avec Mira Voss, des documents internes trouvés dans les bureaux humanisent son parcours (l'éviction au profit d'un cadre mieux connecté) — à découvrir avant le combat ou la séquence de conviction, pour que sa motivation ne soit pas amenée seulement par exposition orale.

## 9. Système de réputation

Une seule variable numérique globale, `VAR_REPUTATION`, initialisée à 0 et modifiée par les choix du joueur tout au long de l'histoire. Elle détermine le ton de l'épilogue (Acte V) — indépendamment des flags de conversion des lieutenants (Terrence/Kess), qui influencent le texte de l'épilogue mais pas ce score.

### Barème des choix

| Moment | Choix | Points |
|---|---|---|
| Cinnabar, réfugiés cachés (Acte I) | Guider vers le bateau | +2 |
| | Cacher sur place | +1 |
| Route 1, objet perdu | Le rendre à la famille | +1 |
| | Ignorer | 0 |
| Viridian, commerçant résistant | Aider (quête d'info) | +1 |
| | Ignorer | 0 |
| Viridian Forest, bûcherons déplacés | Les aider | +1 |
| | Ignorer | 0 |
| Terrence (séquence de conviction) | Convaincu (≥5 pts) | +2 |
| | Combat allégé (3-4 pts) | +1 |
| | Combat complet (≤2 pts) | 0 |
| Mont Sélénite, chercheuse rationaliste | La soutenir | +1 |
| | Ignorer | 0 |
| Cerulean, canalisations | Réparer avant qu'Ondine ne s'engage | +1 |
| Rock Tunnel, mineurs piégés | Les sauver | +2 |
| Vermilion, interception radio | Réussir le puzzle | +1 |
| Zone Safari, Pokémon braconnés | Les libérer | +1 |
| Kess (séquence de conviction) | Convaincue (≥5 pts) | +2 |
| | Combat allégé (3-4 pts) | +1 |
| | Combat complet (≤2 pts) | 0 |
| Fuchsia, débat du clan de Koga | Participer (quelle que soit l'issue) | +1 |
| Céladopole, financement Rocket | Exposer le grand magasin | +2 |
| Saffron, documents sur Mira Voss | Les lire avant la confrontation | +1 |
| Confrontation finale avec Blue | Reconnaître l'injustice sans excuser la méthode | +2 |
| | Rejeter tout en bloc | 0 |
| | Excuser complètement sa méthode | -2 |

**Score maximum théorique** : 22. **Score minimum théorique** : -2 (si tous les choix négatifs/passifs sont pris).

### Paliers et effets sur l'épilogue

- **≤ 8 — Réputation ternie** : les Champions restent distants dans l'épilogue ("on reconnaît la victoire, pas l'homme/la femme"). Cinnabar se reconstruit lentement, sans grande cérémonie. Aucun contenu post-game bonus.
- **9 à 16 — Réputation équilibrée** : épilogue standard, la plupart des Champions présents à la reconstruction de Cinnabar, ton mesuré, ni triomphal ni amer.
- **≥ 17 — Réputation exemplaire** : épilogue chaleureux, tous les Champions présents, scène bonus (Blaine, si retrouvé vivant, témoigne publiquement en faveur du joueur). Débloque un contenu post-game (ex. revanches contre les lieutenants convertis, zone bonus).

### Implémentation technique suggérée

- `VAR_REPUTATION` incrémentée/décrémentée à chaque script de dialogue concerné (`addvar VAR_REPUTATION, X`).
- Vérification du seuil final déclenchée par le flag de fin d'Acte V (`FLAG_BLUE_VAINCU`), qui appelle un script de branchement vers l'un des 3 épilogues selon la valeur de `VAR_REPUTATION`.
- Les flags de conversion (`FLAG_TERRENCE_CONVAINCU`, `FLAG_KESS_CONVAINCUE`) restent lus séparément dans l'épilogue pour adapter le texte (qui apparaît, qui est mentionné) sans changer le palier de réputation lui-même.

## 10. Épilogues et contenu post-game

### Épilogue — Réputation ternie (≤ 8 points)

Cinnabar renaît, mais lentement. Les bateaux reviennent un à un, sans cérémonie. Sur le port reconstruit à la hâte, le joueur retrouve son Arène — vide, silencieuse, comme avant. Pierre, Ondine et les autres Champions envoient un message de félicitations officiel, signé, distant. Aucun d'eux ne fait le déplacement.

Si Blaine a été retrouvé vivant, il reste en retrait, marqué par sa captivité, et n'évoque jamais publiquement ce qui s'est passé. S'il n'a pas été retrouvé, sa disparition reste sans réponse — un vide que même la victoire sur Blue ne comble pas.

Dernière ligne, prononcée par le joueur ou en narration : *"On a gagné. Personne n'est venu le dire."*

Aucun contenu post-game bonus ne se débloque. Les lieutenants non convertis (Terrence, Kess si combattus) disparaissent du jeu sans épilogue individuel.

### Épilogue — Réputation équilibrée (9 à 16 points)

La reconstruction de Cinnabar s'organise avec l'aide ponctuelle de quelques Champions — Pierre envoie des vivres, Erika des ressources pour replanter les zones abîmées du Mont Cinnabar. Ce n'est pas un triomphe collectif, mais une solidarité mesurée, sincère sans être démonstrative.

Si Blaine a été retrouvé, il reprend discrètement sa place de mentor, sans grand discours — un simple "content que tu aies tenu" en guise de reconnaissance. Terrence et Kess, s'ils ont été convaincus, réapparaissent en PNJ recrutables pour des combats optionnels, sans quête dédiée.

Dernière ligne : *"Ça ne répare pas tout. Mais ça repart."*

### Épilogue — Réputation exemplaire (≥ 17 points)

Cinnabar accueille une cérémonie de reconstruction où tous les Champions du Kanto sont présents — la première fois qu'ils se réunissent ainsi depuis des années. Blaine, s'il a été retrouvé, prend la parole publiquement : il raconte sa captivité, salue le joueur, et remet en question ouvertement le silence passé de la Ligue sur les archives Rocket — écho direct à l'accusation de Blue, mais porté cette fois par une voix légitime plutôt que par la violence.

Terrence et Kess, convertis, s'installent comme figures actives de la reconstruction (Terrence supervise la sécurisation du tunnel, Kess obtient la reconnaissance légale de la réserve Safari).

Dernière ligne : *"Pour une fois, Kanto entier a regardé dans la même direction."*

**Contenu post-game débloqué à ce palier** :
- **Revanches renforcées** contre Terrence et Kess (s'ils ont été convertis) — combats optionnels avec des équipes remaniées, plus proches du niveau du joueur en fin de partie plutôt que niveau 34.
- **Zone bonus : les sous-sols du Pokémon Mansion**, inaccessibles pendant la trame principale, contenant les archives complètes que Blue voulait récupérer.
- **Scène optionnelle avec Blue** (en détention) : un dialogue court, non rejouable, où il reconnaît — sans se dédire sur le fond — que la méthode a échoué. Pas de rédemption complète, cohérent avec le ton réaliste retenu depuis le début.

### Boss de la zone bonus : Commandant Kaïn

**Qui il est** : un officier vétéran de la Team Rocket de l'époque de Giovanni, resté seul dans les sous-sols du Mansion depuis la chute de son ancien chef — coupé du monde, il ignorait tout de l'existence de Blue jusqu'à l'incursion du joueur en Acte I. Il croit encore "garder le fort" en attendant des ordres qui ne viendront jamais. Son isolement fait écho volontaire à celui de Cinnabar au début de l'histoire.

**Motivation** : loyauté fanatique à la mémoire de Giovanni plutôt qu'à Blue ou à une idéologie — il méprise d'ailleurs Blue quand il apprend son existence ("un gamin qui joue au chef"). Il protège les archives non pour les détruire ni les révéler, mais parce que "ça ne regarde personne d'autre".

**Découverte en jeu** : le joueur croise des traces de sa présence dès l'Acte I (bruits, portes verrouillées de l'intérieur, rations récentes) sans jamais le rencontrer avant le post-game — un fil discret qui referme un mystère semé dès le début.

**Équipe (niveau 55-60, post-game)** : thème Poison/Ténèbres, doctrine originelle de la Team Rocket — Muk, Weezing, Crobat, Skuntank, Drapion, Tyranitar en pièce finale.

**Réplique d'ouverture** : *"Vingt ans que j'attends un ordre. Le premier visage que je vois, et ce n'est même pas le Boss. Autant en finir proprement."*

**Après défaite** : il ne se rend pas au sens classique — il s'assoit simplement, indifférent à la suite, laissant le joueur libre d'accéder aux archives. Aucune rédemption, aucune conversion : juste la fin d'une attente qui n'avait plus de sens.

## 11. Événements supplémentaires par acte

Ces événements s'insèrent dans la trame existante sans la modifier — ils l'étoffent en ajoutant des respirations, des rebondissements et des liens entre les fils narratifs déjà posés (Blaine, les lieutenants, la réputation).

### Acte I — La chute de Cinnabar

1. **Le choix du premier sauvetage** — pendant l'attaque, le joueur doit choisir entre sécuriser le Centre Pokémon (soigner les blessés) ou les archives de l'Arène (protéger son propre badge/titre). Choix rapide, sans jugement moral appuyé, mais qui teste immédiatement la personnalité qu'on prête au joueur. *(+1 réputation si Centre Pokémon choisi, 0 si Arène — cohérent avec le barème existant.)*
2. **Le carnet du grunt** — un Rocket Grunt en fuite laisse tomber un carnet de codes radio. Objet de lore qui prépare la quête d'interception de Vermilion (section 8) — le joueur comprend plus tard à quoi il servait.
3. **Le Pokémon blessé** — un Pokémon sauvage errant, blessé par les combats, peut être soigné et suit le joueur de façon informelle (pas un membre d'équipe, un "compagnon" cosmétique/narratif) jusqu'à un moment clé de l'Acte V où il joue un petit rôle (aide à localiser Blaine, par exemple).

### Acte II — L'exil et la méfiance

4. **La tempête de Route 21** — la traversée est interrompue par une tempête ; un marin secondaire aide à accoster en urgence. Il réapparaît en Acte V pour organiser le retour par mer vers Cinnabar.
5. **La disparition du Professeur** — à Pallet Town, la rumeur circule que le professeur local s'est terré, redoutant des représailles Rocket. Sous-intrigue qui peut rester non résolue (renforce le sentiment de monde qui se fissure) ou se refermer brièvement en Acte IV.
6. **Le discours de Blue à la radio** — à Viridian, une annonce publique de Blue expose pour la première fois sa position ("la Ligue protège une élite") directement aux habitants. Premier moment où le joueur — et le joueur seul jusque-là — comprend qu'il n'est pas un simple criminel aux yeux de tous.
7. **La photo de Blaine** — à Pewter, un vieux cliché ou une lettre révèle un pan du passé du joueur avec Blaine, humanisant leur lien avant même de savoir s'il est vivant.

### Acte III — L'ampleur du réseau

8. **L'embuscade de reconnaissance** — entre deux zones ouvertes, une petite patrouille Rocket teste le joueur avant les vrais lieutenants (obstacle mineur, pas un boss).
9. **La légende du Mont Sélénite** — fragments de folklore Clefairy trouvés en jeu, qui nourrissent la scène avec Selen sans être indispensables à la comprendre.
10. **La capture d'un Champion allié** — un des Champions déjà ralliés (Pierre ou Ondine, à choisir selon le scénario) est brièvement capturé par la Team Rocket, forçant un petit arc de sauvetage avant de poursuivre l'Acte III. Renforce l'idée que personne n'est à l'abri, y compris les alliés du joueur.
11. **L'éboulement du Rock Tunnel** — juste avant Terrence, un effondrement force une alliance temporaire contre un Pokémon sauvage en furie. Ce moment humanise Terrence avant la séquence de conviction, sans changer son barème de points.

### Acte IV — La reconquête

12. **Le laboratoire caché de la Zone Safari** — découverte d'anciennes installations d'expérimentation héritées de l'ère Giovanni, faisant écho aux carnets de Blaine (Acte I) et annonçant les révélations finales sur les archives du Mansion.
13. **Le subordonné admiratif** — à Céladopole, un dresseur travaillant pour Blue, sincèrement convaincu par son discours mais pas violent, défie le joueur dans un combat optionnel. Sa défaite ébranle ses certitudes sans le convertir complètement — personnage à réutiliser en post-game si souhaité.
14. **La scission du clan de Koga** — après le débat (section 8), une faction minoritaire du clan, en désaccord avec le choix du joueur, tente un sabotage isolé. Conséquence directe et visible du choix fait, sans bloquer la progression.
15. **La piste de Blaine** — à Silph Co, un registre de détention ou un badge confisqué confirme où Blaine est retenu, préparant sa scène de sauvetage en Acte V.
16. **L'alarme de Silph Co** — avant Mira Voss, un passage en infiltration (éviter des patrouilles plutôt que combattre) casse le rythme purement frontal de l'Acte IV.

### Acte V — Confrontation finale

17. **Le sauvetage de Blaine** — scène dédiée, distincte de la simple mention en épilogue : le joueur le retrouve physiquement avant l'assaut final sur le Mansion, ce qui conditionne `FLAG_BLAINE_SAUVE` (utilisé dans les épilogues).
18. **Le retour des alliés** — juste avant la confrontation avec Blue, les lieutenants convertis (Terrence, Kess) et les Champions ralliés apparaissent brièvement pour sécuriser l'accès au Mansion — scène collective qui matérialise concrètement le score de réputation accumulé, avant même de connaître le palier d'épilogue.

## 12. Champions d'arène en devenir (rencontres aléatoires)

**Concept** : en résonance directe avec le thème central de l'histoire — un système de Ligue qui ne reconnaît pas tous les talents également (l'argument même de Blue) — Kanto compte des dresseurs qui s'entraînent pour devenir les **prochains Champions d'Arène**, sur des types encore non représentés officiellement (Vol, Insecte, Combat, Glace, Dragon, Ténèbres, Acier, Fée). Ils apparaissent en **rencontre aléatoire** sur des routes précises, indépendamment de la progression de l'histoire — un joueur peut en croiser dès l'Acte II comme ne jamais en croiser un seul en fin de partie s'il ne s'attarde pas.

Cette mécanique sert directement la durée de vie : contenu optionnel, rejouable, avec un système de revanche à niveau croissant, et un **effet concret sur l'épilogue exemplaire** (voir plus bas).

### Table des 8 aspirants

| Type | Nom | Lieu de rencontre (aléatoire) | Personnalité |
|---|---|---|---|
| Vol | **Ael** | Route 21 (traversée maritime) | Discret, observe les Pokémon Vol en vol libre plutôt que de les capturer par la force — philosophie du dressage "par le respect". |
| Insecte | **Nao** | Viridian Forest | Entomologiste passionné, un peu maladroit socialement, admire en secret l'engagement de Lyre pour la forêt. |
| Combat | **Rook** | Route entre Fuchsia et Saffron | Ancien élève du dojo de Fuchsia, discipline stricte, méprise les dresseurs qui misent tout sur la puissance brute sans technique. |
| Glace | **Iris** | Seafoam Islands (zone glacée canonique, proche de Cinnabar/Fuchsia) | Solitaire, peu bavarde, teste le joueur sans prévenir puis s'en va sans grand discours. |
| Dragon | **Orin** | Route 21 (traversée maritime, rencontre distincte d'Ael) | Fascination quasi religieuse pour les Dragons, discours grandiloquent, sincère plus que théâtral. |
| Ténèbres | **Corvin** | N'importe quelle route, uniquement **de nuit** | Encounter conditionnée à l'heure du jeu si le fork la supporte — renforce le thème par la mécanique elle-même. |
| Acier | **Ferra** | Route de la Centrale (Rock Tunnel) | Ancienne apprentie mécanicienne, pragmatique, parle de ses Pokémon comme de machines de précision. |
| Fée | **Lila** | Céladopole et alentours | Ancienne vendeuse du grand magasin, a quitté son poste après le scandale du financement Rocket (écho à la quête annexe de Céladopole). |

### Mécanique de rencontre

- Sur chaque route concernée, une **vérification aléatoire à faible probabilité** (ex. 1 chance sur 20-30 par entrée sur la carte ou par pas, selon ce que permet le fork) déclenche l'apparition de l'aspirant correspondant, s'il n'a pas déjà été affronté ce jour-ci.
- **Niveau du combat** : calé sur le niveau moyen de l'équipe du joueur au moment de la rencontre (plutôt qu'un niveau fixe), pour rester pertinent quel que soit le moment où le joueur les croise.
- **Revanche** : chaque aspirant peut être réaffronté un nombre limité de fois par semaine de jeu (ou via un flag de cooldown simple), avec une équipe renforcée à chaque victoire du joueur — boucle de progression optionnelle façon "VS Seeker".

### Effet sur l'épilogue exemplaire

Si le joueur atteint le palier de réputation "exemplaire" (section 9) **et** a battu au moins 6 des 8 aspirants au moins une fois, une scène de coda s'ajoute à l'épilogue exemplaire : la Ligue Kanto annonce la création de nouvelles Arènes officielles pour ces 8 types, et les aspirants rencontrés y sont nommés Champions — clôture concrète du thème de la méritocratie porté (à tort, dans sa méthode) par Blue tout au long de l'histoire.

### Implémentation technique suggérée

- Un objet événement invisible ou un script de type "wandering NPC" par route concernée, avec vérification `random(X)` à chaque passage et flag journalier de cooldown (`FLAG_DAILY_<NOM>` remis à zéro par un script de changement de jour si le fork en a un, sinon flag simple à usage unique par visite de map).
- `VAR_ASPIRANTS_BATTUS` : compteur global incrémenté à la première victoire contre chaque aspirant (pas à chaque revanche), utilisé pour la condition de coda (`>= 6`).
- Niveau de l'équipe adverse calculé dynamiquement à partir du niveau moyen de l'équipe du joueur (`getpartyaveragelevel` ou équivalent selon la version du fork) plutôt que codé en dur.

## 13. Points ouverts

Aucun point majeur ne reste en suspens sur la trame, les personnages et les mécaniques narratives. Restent à trancher, si besoin, des détails de mise en œuvre pure (dialogues additionnels, textes de combat, équilibrage précis des niveaux post-game) — au fur et à mesure du scriptage réel dans le fork.

Les 18 événements de la section 11 et le système des 8 aspirants de la section 12 ne sont pas encore scriptés (`scripts/`) — à faire au fur et à mesure, en suivant le même modèle que les autres fichiers (flags de progression, `addvar` sur `VAR_REPUTATION` quand applicable).


