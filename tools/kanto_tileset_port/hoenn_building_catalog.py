"""Catalogue des bâtiments Hoenn déjà présents dans le jeu (suite 38), extraits directement des
map.bin réels des villes non encore modifiées (voir docs/PROJECT_STATUS.md, "État des lieux de
Hoenn"). Chaque entrée a été localisée visuellement sur la preview PNG de sa ville d'origine, puis
sa grille d'IDs de metatile a été lue directement dans le fichier binaire (jamais devinée) via
tools/map_preview/map_preview.py (load_layouts + lecture blockdata brute).

Contrairement au catalogue Kanto (tile_catalog.py, general_frlg/pallet_town_frlg), ces bâtiments
viennent des tilesets HOENN d'origine du moteur (gTileset_General + un tileset secondaire propre
à chaque ville) - split "emerald" (512/512/6), pas "frlg" (640/640/7). Ils ne sont PAS utilisables
tels quels dans une carte Kanto FRLG : ce catalogue sert de référence/inspiration pour dessiner de
nouveaux archétypes, pas une source à copier-coller directement.

Chaque entrée : ville d'origine, tileset (primaire, secondaire), bbox (x, y, largeur, hauteur)
dans la carte source, grille d'IDs (rangée du haut = index 0), et une description de ce qui rend
ce bâtiment visuellement distinct.
"""

HOENN_BUILDINGS = {
    "fortree_treehouse": {
        "town": "FortreeCity",
        "primary_tileset": "gTileset_General",
        "secondary_tileset": "gTileset_Fortree",
        "bbox": (9, 1, 3, 4),  # x, y, largeur, hauteur dans FortreeCity/map.bin
        "grid": [
            [548, 549, 550],
            [556, 557, 558],
            [564, 565, 566],
            [572, 573, 574],
        ],
        "desc": "Cabane perchée sur pilotis, toit de chaume, entrée ouverte sans porte visible "
                "(juste un seuil sombre) — accès uniquement par une échelle au sol.",
    },
    "mossdeep_space_center": {
        "town": "MossdeepCity",
        "primary_tileset": "gTileset_General",
        "secondary_tileset": "gTileset_Mossdeep",
        "bbox": (60, 10, 8, 6),
        "grid": [
            [944, 945, 946, 947, 948, 949, 950, 951],
            [952, 953, 953, 954, 733, 955, 956, 956],
            [960, 961, 961, 962, 733, 963, 964, 964],
            [730, 731, 731, 732, 733, 734, 735, 735],
            [738, 739, 739, 740, 741, 742, 743, 743],
            [746, 747, 747, 748, 749, 750, 751, 751],
        ],
        "desc": "Façade industrielle grise à colonnes de verre bleu, entrée vitrée hexagonale "
                "centrale — dômes d'observatoire (non capturés ici) posés sur le toit.",
    },
    "sootopolis_peaked_house": {
        "town": "SootopolisCity",
        "primary_tileset": "gTileset_General",
        "secondary_tileset": "gTileset_Sootopolis",
        "bbox": (8, 3, 4, 5),
        "grid": [
            [513, 514, 536, 0],   # 0 = hors silhouette (toit pointu, coin vide)
            [521, 522, 523, 597],
            [547, 532, 548, 607],
            [555, 540, 556, 607],
            [582, 0, 583, 604],
        ],
        "desc": "Toit gris pointu façon pagode (silhouette non rectangulaire, coins hauts vides), "
                "mur clair, entrée sombre encadrée — le plus architecturalement singulier des 10.",
    },
    "lavaridge_hot_spring_pc": {
        "town": "LavaridgeTown",
        "primary_tileset": "gTileset_General",
        "secondary_tileset": "gTileset_Lavaridge",
        "bbox": (7, 3, 4, 4),
        "grid": [
            [677, 586, 584, 585],
            [677, 80, 81, 82],
            [685, 88, 89, 90],
            [693, 96, 97, 98],
        ],
        "desc": "Centre Pokémon à toit orange/saumon incurvé (pas le bleu standard) — variante "
                "thermale, façade identique par ailleurs au Centre Pokémon générique.",
    },
    "pacifidlog_raft_hut": {
        "town": "PacifidlogTown",
        "primary_tileset": "gTileset_General",
        "secondary_tileset": "gTileset_Pacifidlog",
        "bbox": (1, 9, 4, 4),
        "grid": [
            [368, 514, 368, 410],  # rangee du fleuron, plus etroite que le corps
            [521, 522, 523, 524],
            [529, 530, 531, 532],
            [537, 538, 539, 540],
        ],
        "desc": "Dôme conique vert (chaume) avec fleuron au sommet, posé sur un radeau de bois "
                "flottant — aucun équivalent Kanto FRLG connu à ce jour.",
    },
    "mauville_modern_tower": {
        "town": "MauvilleCity",
        "primary_tileset": "gTileset_General",
        "secondary_tileset": "gTileset_Mauville",
        "bbox": (7, 10, 6, 4),
        "grid": [
            [729, 729, 729, 729, 732, 264],
            [737, 737, 737, 737, 740, 264],
            [745, 746, 747, 731, 748, 264],
            [722, 723, 730, 739, 738, 264],
        ],
        "desc": "Grande façade rose/violette à fenêtres verticales bleues et porche à colonnes "
                "dorées — style 'salle de jeux' urbaine, la plus large des 10 en un seul bloc.",
    },
    "slateport_oceanic_hall": {
        "town": "SlateportCity",
        "primary_tileset": "gTileset_General",
        "secondary_tileset": "gTileset_Slateport",
        "bbox": (27, 25, 8, 3),
        "grid": [
            [800, 801, 802, 802, 803, 804],
            [808, 809, 810, 810, 811, 812],
            [816, 817, 818, 818, 819, 820],
        ],
        "desc": "Façade arrondie turquoise/blanc à colonnades symétriques, large entrée sombre "
                "centrale — sobre et horizontal, contraste avec les toits pointus des autres.",
    },
    "lilycove_department_store": {
        "town": "LilycoveCity",
        "primary_tileset": "gTileset_General",
        "secondary_tileset": "gTileset_Lilycove",
        "bbox": (23, 0, 9, 7),
        "grid": [
            [768, 749, 750, 769, 763, 769, 749, 750, 770],
            [768, 769, 769, 769, 769, 769, 769, 769, 770],
            [768, 749, 750, 769, 763, 769, 749, 750, 770],
            [768, 769, 769, 769, 769, 769, 769, 769, 770],
            [768, 749, 750, 757, 758, 759, 749, 750, 770],
            [766, 764, 783, 771, 772, 773, 782, 764, 767],
            [774, 765, 727, 779, 780, 781, 726, 765, 775],
        ],
        "desc": "Le seul bâtiment à étages du lot (3 rangées de fenêtres identiques empilées) — "
                "auvents rayés orange au rez-de-chaussée, entrée vitrée à double porte.",
    },
    "verdanturf_cottage": {
        "town": "VerdanturfTown",
        "primary_tileset": "gTileset_General",
        "secondary_tileset": "gTileset_Mauville",
        "bbox": (0, 10, 5, 4),
        "grid": [
            [532, 516, 516, 516, 517],
            [904, 905, 905, 906, 517],
            [912, 913, 913, 914, 845],
            [920, 921, 907, 908, 532],
        ],
        "desc": "Petit cottage à toit vert en bardeaux, mur clair à colombages verts — gabarit "
                "'maison de village' le plus modeste des 10, bon candidat pour une petite ville.",
    },
    "dewford_coastal_house": {
        "town": "DewfordTown",
        "primary_tileset": "gTileset_General",
        "secondary_tileset": "gTileset_Dewford",
        "bbox": (1, 0, 4, 4),
        "grid": [
            [529, 530, 524, 524],
            [531, 532, 532, 532],
            [526, 543, 541, 542],
            [534, 551, 549, 550],
        ],
        "desc": "Toit bleu ardoise à bardeaux, mur crème, porte en bois marron classique — "
                "gabarit générique le plus proche visuellement du PC_STYLE Kanto déjà en jeu.",
    },
}
