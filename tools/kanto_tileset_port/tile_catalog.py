"""Catalogue de metatiles verifies (par rendu + inspection individuelle, jamais par simple
lecture de coordonnees sur une planche) pour general_frlg (primaire) + pallet_town_frlg
(secondaire). Chaque entree a ete confirmee soit visuellement (tools/map_preview render direct,
pas de comptage de colonnes a la main), soit par decodage du comportement FRLG reel
(metatile_attributes.bin), avant d'etre ajoutee ici.

Sert de base a building_archetypes.py et map_builder.py. Voir docs/TECHNICAL_ARCHITECTURE.md
pour la methode complete.
"""

# --- Terrain (tous verifies : carrelage 4x4/5x5 sans artefact avant adoption) ---
TERRAIN = {
    "grass": 1,        # id=0 est un metatile vide/non defini, PAS de l'herbe
    "path": 258,
    "sand": 503,
    "water": 522,       # id=512 rejete : bordure de sable integree dans la texture, cree des rayures au carrelage
    "hill": 120,
    "tall_grass": 10,
    "tree_bush": 26,    # general_frlg, buisson rond
    "tree_pine": 683,   # pallet_town_frlg, pin plus anguleux (a placer en petits groupes, pas en bloc plein)
    "flower": 4,
    "hedge": 250,       # haie ronde, pour bordures de jardin devant les maisons
}

# --- Portes verifiees (comportement decode directement depuis metatile_attributes_emerald_
#     behaviors.bin, pas suppose) - une porte est un warp_event place sur ces coordonnees, le
#     comportement de metatile ne fait que jouer une animation/interaction, il ne warp pas lui-meme ---
DOORS = {
    "pokeball_door_grey": 339,   # porte a double battant, embleme Poke Ball gris/blanc, MB_NORMAL
    "pokeball_door_red": 90,     # porte a double battant, embleme Poke Ball rouge (style Mart), MB_NORMAL
    "pc_style_door": 61,         # porte bois du PC_STYLE, MB_ANIMATED_DOOR (verifiee suite 36)
    "modern_style_door": 401,    # porte vitree du MODERN_STYLE, MB_NON_ANIMATED_DOOR (verifiee suite 39)
    "orange_roof_door": 675,     # porte du ORANGE_ROOF_STYLE (pallet_town_frlg), MB_ANIMATED_DOOR (suite 39)
}

# --- Archetype PC_STYLE (toit bleu, dortoir a embleme Poke Ball) : 4 cases de large.
# Corrige en suite 36 (retour de Thomas sur des references de vraie qualite Pokemon) : la
# version d'origine repetait la meme tuile de toit plate 4x (41,41,41,41), ce qui rendait un
# toit plat sans coins ni gouttiere - au lieu de la vraie sequence a 4 rangees du tileset
# (coins arrondis + gouttiere + dortoir), verifiee tuile par tuile par rendu direct. La porte
# (id 61) porte le comportement MB_ANIMATED_DOOR (verifie via metatile_attributes_emerald_
# behaviors.bin), pas juste MB_NORMAL - porte fonctionnelle authentique, pas un pan de mur. ---
PC_STYLE = {
    "roof_top": [40, 41, 42, 43],            # coin arrondi gauche, plat x2, coin/encoche droite
    "roof_gutter": [52, 53, 54, 55],         # gouttiere/bordure grise sous le toit (rangee manquante avant)
    "roof_dormer": [56, 57, 58, 59],         # dortoir avant avec embleme Poke Ball, coins arrondis
    "wall_door_row": [60, "DOOR", 62, 63],   # "DOOR" remplace par pokeball_door_grey (id 61, MB_ANIMATED_DOOR)
}

# --- Archetype MART_STYLE (toit rouge brique, porte a embleme rouge) : 4 cases de large ---
MART_STYLE = {
    "roof_top": [72, 73, 74, 75],
    "roof_bottom": [80, 81, 82, 83],
    "wall_door_row": [88, 89, 90, 91],      # porte (id 90, embleme rouge) deja integree a cette rangee
}

# --- Archetype GYM_STYLE (brique claire + fenetres grises) : 4 cases de large.
#     Tuile 337 (texte "GYM" incruste) et 320 (comportement EAST_ARROW_WARP special) EXCLUES
#     volontairement : la premiere serait une incoherence de traduction, la seconde a un
#     comportement de script qui n'a rien a faire sur un mur decoratif.
#     Corrige en suite 36 : l'ancien wall_door_row avait DEUX placeholders "DOOR" cote a cote,
#     ce qui rendait deux portes identiques visibles l'une a cote de l'autre (une seule etait
#     reellement fonctionnelle) - repere en comparant au rendu direct. Une seule porte, franche.
#     Re-corrige en suite 37 : la tuile 328 (fenetre + bac a fleurs) empilee 3x d'affilee sur la
#     meme colonne produisait un motif de fenetres identiques repetees, incoherent pour une
#     facade reelle - remplacee par 329/331 (poteaux d'angle) + 330 (brique pleine). ---
GYM_STYLE = {
    "roof_top": [321, 322, 323, 323],
    "wall": [329, 330, 330, 331],             # 329/331 = poteaux d'angle, 330 = brique pleine
    "wall_door_row": [329, "DOOR", 330, 331],  # "DOOR" remplace par pokeball_door_grey (id 339)
}

# --- Facade du Laboratoire (pallet_town_frlg, motif brique/fenetres bleues arrondies deja
#     valide en suite 33 - reconduit tel quel) : 7 cases de large. Porte en DERNIERE rangee
#     (niveau du sol, comme un vrai batiment), pas au milieu de la facade. ---
LAB_STYLE = {
    "top": [704, 705, 706, 707, 708, 709, 704],
    "wall": [712, 713, 714, 715, 716, 712, 713],
    "wall_door_row": [712, 713, 714, "DOOR", "DOOR", 715, 712],
}

# --- Archetype MODERN_STYLE (facade grise a fenetres bleues, style batiment urbain/magasin) :
#     3 cases de large. Trouve en suite 39 en comparant le tileset general_frlg deja dans le
#     moteur a une ROM FireRed externe (Pokemon FireRed Rocket Edition) fournie par Thomas - les
#     deux se sont reveles identiques (meme tileset FRLG d'origine), donc cet archetype est
#     construit directement depuis nos propres tuiles general_frlg, pas extrait de la ROM.
#     Porte (id 401) verifiee MB_NON_ANIMATED_DOOR - fonctionnelle mais sans animation
#     d'ouverture (contrairement a pc_style_door). Valide par Thomas (proposition #1). ---
MODERN_STYLE = {
    "wall_windows": [384, 385, 386],              # bande de 2 fenetres bleues
    "wall_mid": [387, 388, 389],                  # mur gris plein
    "wall_door_row": [400, "DOOR", 402],          # "DOOR" remplace par modern_style_door (id 401)
}

# --- Archetype ORANGE_ROOF_STYLE (toit orange a rainures + mur gris/fenetres bleues) :
#     4 cases de large. Tuiles issues de pallet_town_frlg (secondaire, offsets 640+) - donc,
#     comme LAB_STYLE, valable uniquement quand pallet_town_frlg est le tileset secondaire de
#     la carte (pas universel comme PC/MART/GYM_STYLE qui sont sur general_frlg primaire).
#     Trouve et valide en suite 39 (proposition #2), meme contexte que MODERN_STYLE ci-dessus. ---
ORANGE_ROOF_STYLE = {
    "roof_top": [649, 650, 650, 651],             # toit rainure, 3 tuiles reelles + 650 reprise en padding
    "roof_eave": [657, 658, 658, 659],            # bordure de toit avec liseret rouge
    "wall_windows": [665, 666, 667, 668],
    "wall_door_row": [673, 674, "DOOR", 676],     # "DOOR" remplace par orange_roof_door (id 675)
}

# --- Objets decoratifs autonomes (1 case), general_frlg primaire - poser avec IMPASSABLE comme
#     les autres decors, jamais PASSABLE (ce ne sont pas des portes). Valides par Thomas
#     (propositions #5 et #8) en suite 39. ---
PROPS = {
    "gym_console": 360,     # borne d'arcade rouge/grise avec texte "GYM" incruste
    "statue_sign": 3,       # petit panneau/totem a visage sculpte
}

# --- Decor secondaire viridian_city_frlg (suite 35, preview d'une grande ville) : offsets deja
# ajoutes (640 + index local, frontiere secondaire du split "frlg"). Verifie par rendu direct
# individuel (tools/map_preview, grille labelisee) - PAS de batiment reconstruit depuis ce
# tileset pour l'instant, seulement du decor/relief pour habiller PC/MART/GYM_STYLE (qui restent
# des tuiles general_frlg PRIMAIRES, donc valables quel que soit le tileset secondaire choisi). ---
VIRIDIAN_DECOR = {
    "tree_big": 640 + 5,       # grand arbre rond, plus dense que TERRAIN["tree_bush"]
    "hedge_row": 640 + 8,      # haie/rangee de buisson taille, en bande
    "rock_patch": 640 + 14,    # sol rocheux/terre (transition, pas relief pur)
    "cliff_face": 640 + 22,    # paroi rocheuse, pour un fond de montagne
}
