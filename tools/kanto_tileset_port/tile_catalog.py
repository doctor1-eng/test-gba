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

# --- Portes verifiees walkable (MB_FRLG_NORMAL, pas de comportement special -
#     une porte est un warp_event place sur ces coordonnees, pas un comportement de metatile) ---
DOORS = {
    "pokeball_door_grey": 339,   # porte a double battant, embleme Poke Ball gris/blanc
    "pokeball_door_red": 90,     # porte a double battant, embleme Poke Ball rouge (style Mart)
}

# --- Archetype PC_STYLE (toit bleu, dortoir a embleme Poke Ball) : 4 cases de large ---
PC_STYLE = {
    "roof_top": [41, 41, 41, 41],           # bande de toit plate, tuilable
    "roof_dormer": [56, 57, 58, 59],        # dortoir avant avec embleme Poke Ball, coins arrondis
    "wall": [32, 32, 32, 32],               # mur/fenetres
    "wall_door_row": [32, "DOOR", "DOOR", 32],  # "DOOR" remplace par pokeball_door_grey au moment de la construction
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
#     comportement de script qui n'a rien a faire sur un mur decoratif. ---
GYM_STYLE = {
    "roof_top": [321, 322, 323, 321],
    "wall": [328, 329, 330, 331],
    "wall_door_row": [329, "DOOR", "DOOR", 329],  # "DOOR" remplace par pokeball_door_grey
}

# --- Facade du Laboratoire (pallet_town_frlg, motif brique/fenetres bleues arrondies deja
#     valide en suite 33 - reconduit tel quel) : 7 cases de large. Porte en DERNIERE rangee
#     (niveau du sol, comme un vrai batiment), pas au milieu de la facade. ---
LAB_STYLE = {
    "top": [704, 705, 706, 707, 708, 709, 704],
    "wall": [712, 713, 714, 715, 716, 712, 713],
    "wall_door_row": [712, 713, 714, "DOOR", "DOOR", 715, 712],
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
