// Heart & Soul - Acte I : la chute de Cinnabar
// Enchaine directement (goto, meme execution de script) a la fin de
// HeartSoul_EventScript_ChooseTeam_Feu (data/scripts/heart_and_soul_intro.inc), une fois
// l'equipe de depart donnee. Se termine par "end", pas "return" : appele par goto, pas call.
//
// Revision 2026-09-03 (3e passe - corrige un blocage constate en test) : la 2e passe faisait
// un warp vers CinnabarIsland_hns puis continuait la CHOREGRAPHIE DANS LE MEME SCRIPT juste
// apres le "waitstate" du warp. Teste en jeu : le joueur arrive bien sur la carte exterieure,
// mais plus rien ne se passe ensuite (aucun mouvement, aucun texte). Cause identifiee a
// posteriori : ce depot documente deja EXACTEMENT ce probleme ailleurs sur cette meme carte
// (voir le commentaire au-dessus de CinnabarIsland_EventScript_Blaine,
// data/maps/CinnabarIsland_hns/scripts.inc - "un ancien essai [de declencher un msgbox/lockall]
// depuis CinnabarIsland_OnTransition [...] retire, car ce point d'accroche tourne avant que
// l'ecran soit pret pour des fenetres/menus"). Continuer un script avec applymovement/msgbox
// immediatement apres un warp scripte se heurte a la meme fenetre "ecran pas encore pret" - le
// warp lui-meme n'est pas le probleme (le joueur arrive bien), c'est la suite immediate dans le
// meme run de script qui ne s'execute pas de facon fiable.
//
// Solution retenue, alignee sur le motif deja EPROUVE dans ce depot pour ce genre de probleme
// (HeartSoul_EventScript_CinnabarVerrouilleeCheck juste plus bas, et le commentaire cite plus
// haut) : la choregraphie ne continue plus dans CE script apres le warp. Elle repart depuis
// MAP_SCRIPT_ON_FRAME_TABLE de CinnabarIsland_hns (CinnabarIsland_OnFrame, voir
// data/maps/CinnabarIsland_hns/scripts.inc), qui tourne UNE FOIS l'ecran pret, contrairement a
// ON_TRANSITION. HeartSoul_EventScript_CinnabarAttack se contente donc de l'alarme + du warp,
// puis "end" immediatement apres le "waitstate" (meme motif que le warp final de ce script vers
// Pallet Town, deja eprouve) ; HeartSoul_EventScript_CinnabarAttackPart2Check (declenche depuis
// la carte) detecte l'arrivee et lance HeartSoul_EventScript_CinnabarAttackPart2, qui contient
// toute la choregraphie + le combat + la suite du scenario (choix refugies, etc.), inchangee
// par ailleurs. FLAG_ATTAQUE_CINNABAR_PART2_LANCEE evite tout rejeu si ON_FRAME_TABLE est
// re-evalue en cours de route (ex. au retour du combat).
//
// Position de warp (37,31) et position de depart du Grunt (41,28) verifiees par decodage de
// data/layouts/CinnabarIsland_hns/map.bin (collision nulle sur les deux cases et sur tout le
// chemin de marche utilise plus bas). Le Grunt visible pendant la choregraphie EST le meme qui
// affronte ensuite le joueur (pas un second Grunt distinct) : narrativement plus simple (Blaine
// gagne du temps face a CE Grunt precis, qui se retourne ensuite contre le joueur une fois
// Blaine hors-jeu) et techniquement plus simple (un seul object_event, un seul trainer).
// 3e correction (retour utilisateur : Grunt maintenant visible ET mobile, mais Blaine
// toujours invisible) : FLAG_HIDE_CINNABAR_BLAINE est pose par defaut des la creation de la
// partie (data/scripts/new_game.inc, gros bloc "setflag FLAG_HIDE_<Champion>" - Blaine, comme
// tous les autres Champions, reste cache jusqu'a son evenement de reapparition prevu par le
// jeu de base, ici HeartSoul_EventScript_DeclencherEpilogue/ViridianCity_Gym_hns en toute fin
// de scenario). Contrairement au Grunt (jamais cache par ce chantier), Blaine EST donc bel et
// bien cache au moment ou CinnabarIsland_hns charge ses object_events - meme cause racine que
// le bug du Grunt corrige plus haut (clearflag apres le chargement de la carte n'aurait rien
// change). Corrige en revelant Blaine ICI, avant le warp, pendant que l'ecran est encore noir -
// il est donc deja visible (flag clair) au moment ou la carte charge ses objets. Il est re-cache
// plus bas dans HeartSoul_EventScript_CinnabarAttackPart2 quand il "disparait" dans la
// choregraphie (setflag + removeobject, inchange) - coherent avec le reste du jeu ou il reste
// cache jusqu'a l'epilogue.
HeartSoul_EventScript_CinnabarAttack::
	lockall
	setflag FLAG_ATTAQUE_CINNABAR_LANCEE
	clearflag FLAG_HIDE_CINNABAR_BLAINE
	msgbox HeartSoul_Text_AttaqueDebut, MSGBOX_DEFAULT
	closemessage
	fadescreenswapbuffers FADE_TO_BLACK
	warp MAP_CINNABAR_ISLAND_HNS, 37, 31
	waitstate
	end

// Declenche par CinnabarIsland_OnFrame (MAP_SCRIPT_ON_FRAME_TABLE, VAR_TEMP_1) une fois l'ecran
// pret apres le warp ci-dessus. VAR_TEMP_1 est mis a 1 sur les deux branches, meme motif
// defensif que HeartSoul_EventScript_CinnabarVerrouilleeCheck (desarme la condition jusqu'au
// prochain chargement de carte, sans quoi ce script tournerait en boucle chaque frame).
HeartSoul_EventScript_CinnabarAttackPart2Check::
	goto_if_unset FLAG_ATTAQUE_CINNABAR_LANCEE, HeartSoul_CinnabarAttackPart2Check_Skip
	goto_if_set FLAG_ATTAQUE_CINNABAR_PART2_LANCEE, HeartSoul_CinnabarAttackPart2Check_Skip
	setflag FLAG_ATTAQUE_CINNABAR_PART2_LANCEE
	setvar VAR_TEMP_1, 1
	goto HeartSoul_EventScript_CinnabarAttackPart2
HeartSoul_CinnabarAttackPart2Check_Skip:
	setvar VAR_TEMP_1, 1
	end

// La choregraphie proprement dite, plus le combat force et la suite deja existante du scenario
// (choix refugies/Pokemon blesse/etc., inchangee depuis la 1ere passe). trainerbattle_single
// appele directement ici (pas depuis le script d'interaction de l'objet, inutilise) : verifie
// au prealable que ce macro ne necessite aucun object_event pour fonctionner (LOCALID_NONE cote
// macro, asm/macros/event.inc) - mais ici l'objet est bel et bien present et visible, donc le
// combat demarre avec le bon sprite a l'ecran. Forme a 3 arguments (pas de victory script) : le
// combat termine, l'execution continue simplement a la ligne suivante.
//
// Correction (retour utilisateur : "aucun sprite de Blaine ou de Team Rocket") : le Grunt
// n'etait plus cache par defaut via FLAG_HIDE_CINNABAR_GRUNT_INTRO puis "clearflag" ici pour le
// reveler - cause identifiee dans le code moteur (TrySpawnObjectEvents,
// src/event_object_movement.c : "!FlagGet(template->flagId)" n'est evalue QU'AU MOMENT ou la
// carte est chargee/l'objet entre dans le champ de la camera, pas en continu ; clearflag apres
// coup ne fait donc pas apparaitre retroactivement un objet deja ignore a ce moment-la).
// Corrige en laissant le Grunt visible des le depart (flag jamais pose avant ce point), meme
// convention que Blaine et tous les autres PNJ de ce projet - FLAG_HIDE_CINNABAR_GRUNT_INTRO
// reste utilise plus bas pour le faire disparaitre apres le combat (setflag + removeobject,
// motif deja eprouve avec Blaine).
//
// 2e correction (retour utilisateur suivant : Grunt visible mais immobile) : ce script tourne
// sur MAP_SCRIPT_ON_FRAME_TABLE, qui s'execute chaque frame independamment du fondu natif
// d'entree du warp (WarpFadeInScreen/FieldCB_DefaultWarpExit, src/field_screen_effect.c,
// declenche automatiquement par DoWarp() une fois la nouvelle carte chargee). Un simple delay
// de 20 frames (essai precedent) n'etait probablement pas suffisant pour couvrir tout le temps
// de ce fondu + la mise en place native du warp (SetUpWarpExitTask) : la choregraphie
// s'executait donc bien, mais en grande partie PENDANT que l'ecran etait encore noir/en train
// de s'eclaircir - invisible au joueur, pas absente du script. Delay porte a 60 frames (~1
// seconde) par securite, largement au-dela de la duree d'un fondu standard, pour garantir que
// l'ecran est completement visible avant le premier applymovement.
HeartSoul_EventScript_CinnabarAttackPart2::
	lockall
	delay 60
	applymovement LOCALID_CINNABAR_GRUNT_INTRO, HeartSoul_Movement_GruntEntree
	waitmovement 0
	applymovement LOCALID_CINNABAR_BLAINE, HeartSoul_Movement_BlaineReagit
	waitmovement 0
	msgbox HeartSoul_Text_BlaineReagit, MSGBOX_DEFAULT
	applymovement LOCALID_CINNABAR_BLAINE, HeartSoul_Movement_BlaineSeplaceDevant
	waitmovement 0
	msgbox HeartSoul_Text_BlaineSeplaceDevant, MSGBOX_DEFAULT
	applymovement LOCALID_PLAYER, HeartSoul_Movement_JoueurRecule
	waitmovement 0
	playse SE_THUNDER
	fadescreenswapbuffers FADE_TO_WHITE
	setflag FLAG_HIDE_CINNABAR_BLAINE
	removeobject LOCALID_CINNABAR_BLAINE
	setflag FLAG_BLAINE_DISPARU
	setflag FLAG_HIDE_CINNABAR_GRUNT_INTRO
	removeobject LOCALID_CINNABAR_GRUNT_INTRO
	fadescreenswapbuffers FADE_FROM_WHITE
	msgbox HeartSoul_Text_EclatCombat, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_BlaineDisparu, MSGBOX_DEFAULT
	applymovement LOCALID_CINNABAR_GRUNT2, HeartSoul_Movement_Grunt2Approche
	waitmovement 0
	trainerbattle_single TRAINER_ROCKET_GRUNT_CINNABAR_HNS, HeartSoul_Text_GruntSeen, HeartSoul_Text_GruntBeaten, HeartSoul_EventScript_CinnabarAttackGrunt2Victoire
	end

// Retour utilisateur : apres la victoire sur le 2e Grunt, "on ne peut pas reparler au grunt et
// rien ne se declenche" - la suite du scenario (choix refugies, etc.) ne s'enchainait pas.
// Cause : trainerbattle_single a 3 arguments (sans script de victoire) utilise TRAINER_BATTLE_
// SINGLE (asm/macros/event.inc), un type de combat concu pour l'approche normale d'un PNJ
// (walk-up), pas pour un combat force sans object_event associe (LOCALID_NONE cote macro,
// comme documente plus haut). Sans objet reellement approchant, ce type ne rend pas la main de
// facon fiable au script apres la victoire. Meme correction que celle deja appliquee avec
// succes aux evenements 8/13 de la section 11 (Route3_EventScript_PatrouilleRecon,
// CeladonCity_EventScript_SubordonneAdmiratif) : forme a 4 arguments (TRAINER_BATTLE_CONTINUE_
// SCRIPT), qui redirige explicitement vers un script de victoire au lieu de compter sur un
// retour implicite.
HeartSoul_EventScript_CinnabarAttackGrunt2Victoire::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox HeartSoul_Text_GruntApresBattle, MSGBOX_DEFAULT
	setflag FLAG_HIDE_CINNABAR_GRUNT2
	removeobject LOCALID_CINNABAR_GRUNT2
	msgbox HeartSoul_Text_SauvetageIntro, MSGBOX_DEFAULT
HeartSoul_CinnabarAttack_SauvetageChoice:
	dynmultipush HeartSoul_Text_SauvetageCentre, 0
	dynmultipush HeartSoul_Text_SauvetageArene, 1
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, HeartSoul_CinnabarAttack_SauvetageCentre
	case 1, HeartSoul_CinnabarAttack_SauvetageArene
	goto HeartSoul_CinnabarAttack_SauvetageChoice
HeartSoul_CinnabarAttack_SauvetageCentre:
	addvar VAR_REPUTATION, 1
	msgbox HeartSoul_Text_SauvetageCentreResultat, MSGBOX_DEFAULT
	goto HeartSoul_CinnabarAttack_CarnetGrunt
HeartSoul_CinnabarAttack_SauvetageArene:
	msgbox HeartSoul_Text_SauvetageAreneResultat, MSGBOX_DEFAULT
// Suite de l'Acte I mise en scene (retour utilisateur : "continue de realiser l'histoire avec
// des sprites visibles jusqu'a Pallet Town"). Refugies et Pokemon blesse restent sur
// CinnabarIsland_hns (aucune carte "grotte du Mont Cinnabar" separee n'existe dans ce fork -
// verifie par listing de data/maps/ avant de commencer, voir technical_map.md - le joueur
// traverse simplement une autre partie de la meme zone rocheuse ou se joue deja la
// choregraphie Blaine/Grunt). Meme regle deja eprouvee 3 fois sur cette carte : les deux PNJ
// (LOCALID_CINNABAR_REFUGEE, LOCALID_CINNABAR_POKEMON_BLESSE) ne sont jamais caches avant leur
// scene - ils existent des le chargement de la carte, simplement hors du champ initial de la
// choregraphie, et sont retires par setflag+removeobject une fois leur scene jouee. Positions
// et chemins de marche verifies par decodage de data/layouts/CinnabarIsland_hns/map.bin.
// Reconciliation du document de design detaille : le carnet n'est plus ramasse par simple
// narration, un GRUNT en fuite (jamais cache avant sa scene, meme motif que les autres) traverse
// reellement l'ecran (rangee y=30, terrain sec verifie - voir HeartSoul_Movement_GruntFuite plus
// bas) avant de disparaitre, puis le texte du carnet s'affiche comme s'il venait de le laisser
// tomber en courant.
HeartSoul_CinnabarAttack_CarnetGrunt:
	applymovement LOCALID_CINNABAR_GRUNT_FUITE, HeartSoul_Movement_GruntFuite
	waitmovement 0
	setflag FLAG_HIDE_CINNABAR_GRUNT_FUITE
	removeobject LOCALID_CINNABAR_GRUNT_FUITE
	msgbox HeartSoul_Text_CarnetGrunt, MSGBOX_DEFAULT
	applymovement LOCALID_PLAYER, HeartSoul_Movement_VersRefugies
	waitmovement 0
	msgbox HeartSoul_Text_RefugiesIntro, MSGBOX_DEFAULT
HeartSoul_CinnabarAttack_RefugiesChoice:
	dynmultipush HeartSoul_Text_RefugiesGuider, 0
	dynmultipush HeartSoul_Text_RefugiesCacher, 1
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, HeartSoul_CinnabarAttack_RefugiesGuider
	case 1, HeartSoul_CinnabarAttack_RefugiesCacher
	goto HeartSoul_CinnabarAttack_RefugiesChoice
HeartSoul_CinnabarAttack_RefugiesGuider:
	setflag FLAG_REFUGIES_GUIDES
	addvar VAR_REPUTATION, 2
	msgbox HeartSoul_Text_RefugiesGuiderResultat, MSGBOX_DEFAULT
	goto HeartSoul_CinnabarAttack_Verrouillage
HeartSoul_CinnabarAttack_RefugiesCacher:
	setflag FLAG_REFUGIES_CACHES
	addvar VAR_REPUTATION, 1
	msgbox HeartSoul_Text_RefugiesCacherResultat, MSGBOX_DEFAULT
HeartSoul_CinnabarAttack_Verrouillage:
	setflag FLAG_HIDE_CINNABAR_REFUGEE
	removeobject LOCALID_CINNABAR_REFUGEE
	setflag FLAG_HIDE_CINNABAR_REFUGEE_ENFANT
	removeobject LOCALID_CINNABAR_REFUGEE_ENFANT
	setflag FLAG_CINNABAR_VERROUILLEE
	setflag FLAG_ACTE_1_TERMINE
	setflag FLAG_OBJET_PERDU_TROUVE
	applymovement LOCALID_PLAYER, HeartSoul_Movement_VersPokemonBlesse
	waitmovement 0
	msgbox HeartSoul_Text_PokemonBlesseIntro, MSGBOX_DEFAULT
HeartSoul_CinnabarAttack_PokemonBlesseChoice:
	dynmultipush HeartSoul_Text_PokemonBlesseSoigner, 0
	dynmultipush HeartSoul_Text_PokemonBlesseIgnorer, 1
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, HeartSoul_CinnabarAttack_PokemonBlesseSoigner
	case 1, HeartSoul_CinnabarAttack_PokemonBlesseIgnorer
	goto HeartSoul_CinnabarAttack_PokemonBlesseChoice
HeartSoul_CinnabarAttack_PokemonBlesseSoigner:
	setflag FLAG_POKEMON_BLESSE_SAUVE
	addvar VAR_REPUTATION, 1
	msgbox HeartSoul_Text_PokemonBlesseSoignerResultat, MSGBOX_DEFAULT
	goto HeartSoul_CinnabarAttack_ObjetTrouve
HeartSoul_CinnabarAttack_PokemonBlesseIgnorer:
	msgbox HeartSoul_Text_PokemonBlesseIgnorerResultat, MSGBOX_DEFAULT
HeartSoul_CinnabarAttack_ObjetTrouve:
	setflag FLAG_HIDE_CINNABAR_POKEMON_BLESSE
	removeobject LOCALID_CINNABAR_POKEMON_BLESSE
	msgbox HeartSoul_Text_ObjetTrouve, MSGBOX_DEFAULT
	applymovement LOCALID_PLAYER, HeartSoul_Movement_VersPort
	waitmovement 0
	msgbox HeartSoul_Text_FuiteRoute21, MSGBOX_DEFAULT
	closemessage
	fadescreenswapbuffers FADE_TO_BLACK
	warp MAP_CINNABAR_ISLAND_BOAT_CABIN_HNS, 8, 7
	waitstate
	end

// -----------------------------------------------------------------------------------------
// Scene de la tempete/du marin (fin de l'Acte I). Retour utilisateur : le joueur se
// retrouvait en mode Surf au milieu de l'eau plutot que sur un bateau (les coordonnees
// choisies pour le Pokemon blesse et pour Route21_hns etaient en realite de l'eau navigable,
// pas de la terre ferme - collision seule (MAPGRID_COLLISION_MASK) ne distingue pas terre et
// eau, il faut aussi lire le comportement du metatile (METATILE_ATTR_BEHAVIOR_MASK) ; verifie
// apres coup par decodage croise de map.bin + metatile_attributes.bin des tilesets
// gTileset_Kanto_General_Hns/gTileset_Lavaridge_Hns - (38,36) et la zone de Route21_hns
// choisie etaient bien MB_OCEAN_WATER). Le Pokemon blesse a ete repositionne en (38,35), mais
// cette premiere correction s'est averee ELLE AUSSI fausse (seuil de decodage primaire/
// secondaire errone - voir le commentaire au-dessus de HeartSoul_Movement_VersPokemonBlesse
// plus bas pour le detail) : la case etait toujours de l'eau. Reposition finale en (42,34),
// verifiee avec le bon seuil. La scene de la tempete est deplacee vers une VRAIE carte
// d'interieur de bateau plutot que l'exterieur de Route21_hns (qui est presque entierement
// maritime - y
// mettre en scene un marin "sur le pont" aurait nécessité que le joueur y navigue en Surf, ce
// qui ne correspond pas a "vous etes sur un bateau").
//
// MAP_CINNABAR_ISLAND_BOAT_CABIN_HNS (nouvelle carte, data/maps/CinnabarIsland_BoatCabin_hns) :
// PAS de tuiles inventees - layout.bin/border.bin dupliques tels quels depuis
// CinnabarIsland_Mansion_Hns (piece unique 13x10 deja construite et testee ce meme chantier),
// meme paire de tilesets (gTileset_Johto_Building_Hns/gTileset_House_Lab_Hns), deja confirmes
// compiles sous IS_HNS. Verifie avant de reutiliser une geometrie _Frlg existante (ex.
// SSAnne_Deck_Frlg) que ses tilesets (gTileset_General_Frlg/gTileset_SSAnne,
// src/data/tilesets/headers.h lignes 847-1542) sont exclus des builds POKEMON_HNS - meme piege
// deja documente dans technical_map.md pour les interieurs Cinnabar - donc pas reutilisable
// directement ; d'ou la duplication d'une geometrie deja native _Hns plutot qu'un warp externe
// vers du contenu _Frlg.
//
// Declenchee depuis MAP_SCRIPT_ON_FRAME_TABLE (CinnabarIsland_BoatCabin_hns/scripts.inc) - meme
// motif eprouve que la choregraphie Cinnabar/Route21 : le warp scripte ci-dessus se contente de
// "waitstate" + "end", la suite reprend une fois l'ecran reellement pret. FLAG_MARIN_ROUTE21_
// RENCONTRE (deja alloue, section 11 evenement 4) reutilise comme garde anti-rejeu.
// -----------------------------------------------------------------------------------------
HeartSoul_EventScript_TempeteBoatCabinCheck::
	goto_if_unset FLAG_ATTAQUE_CINNABAR_LANCEE, HeartSoul_TempeteBoatCabinCheck_Skip
	goto_if_set FLAG_MARIN_ROUTE21_RENCONTRE, HeartSoul_TempeteBoatCabinCheck_Skip
	setvar VAR_TEMP_0, 1
	goto HeartSoul_EventScript_TempeteBoatCabinScene
HeartSoul_TempeteBoatCabinCheck_Skip:
	setvar VAR_TEMP_0, 1
	end

HeartSoul_EventScript_TempeteBoatCabinScene::
	lockall
	delay 60
	playse SE_THUNDERSTORM
	msgbox HeartSoul_Text_TempeteRoute21Intro, MSGBOX_DEFAULT
	applymovement LOCALID_CINNABAR_BOAT_MARIN, HeartSoul_Movement_MarinApproche
	waitmovement 0
	msgbox HeartSoul_Text_TempeteRoute21, MSGBOX_DEFAULT
	setflag FLAG_MARIN_ROUTE21_RENCONTRE
	closemessage
	fadescreenswapbuffers FADE_TO_BLACK
	warp MAP_PALLET_TOWN_HNS, 6, 8
	waitstate
	end

// ---------------------------------------------------------------------------------------
// Mouvements de la choregraphie d'ouverture (Grunt/Blaine/joueur sur CinnabarIsland_hns).
// Meme convention que le reste du fork : label + actions de mouvement + step_end
// (data/scripts/movement.inc, asm/macros/movement.inc).
// ---------------------------------------------------------------------------------------
HeartSoul_Movement_GruntEntree:
	walk_left
	walk_left
	walk_left
	face_left
	step_end

HeartSoul_Movement_BlaineReagit:
	emote_exclamation_mark
	face_right
	step_end

HeartSoul_Movement_BlaineSeplaceDevant:
	walk_down
	walk_down
	face_down
	step_end

HeartSoul_Movement_JoueurRecule:
	walk_right
	face_left
	step_end

// Evenement 0bis (retour utilisateur) : un 2e Grunt distinct, jamais cache avant ce point (meme
// motif deja eprouve pour le 1er Grunt et pour Blaine - un objet qui doit etre visible/mobile
// des le debut de la choregraphie ne peut pas etre revele par un clearflag apres coup, cause
// racine documentee plus haut), qui marche vers le joueur juste apres l'eclair pour engager le
// combat. Position de depart (44,31) et chemin de marche verifies par decodage de
// data/layouts/CinnabarIsland_hns/map.bin (collision nulle, meme rangee que le joueur apres son
// recul).
HeartSoul_Movement_Grunt2Approche:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	face_left
	step_end

// Suite Acte I mise en scene (refugies/Pokemon blesse/port) - joueur en (38,31) apres le
// combat contre le 2e Grunt. Chemins verifies par decodage de map.bin, aucun chevauchement
// avec les object_events deja presents.
HeartSoul_Movement_VersRefugies:
	walk_right
	walk_down
	walk_down
	face_down
	step_end

// 2e correctif (reconciliation du document de design detaille) : la 9e passe avait deja
// deplace le Pokemon blesse de (38,36) vers (38,35) en le croyant sur terre ferme, mais ce
// diagnostic reposait sur un decodage errone - le seuil primaire/secondaire utilise etait
// 512 (convention Emerald standard), alors que ce fork (MODERN=1, NUM_METATILES_IN_PRIMARY,
// include/fieldmap.h) utilise en realite 640 metatiles primaires. Tout metatile d'ID compris
// entre 512 et 639 etait donc lu dans le mauvais tileset secondaire (ou hors limites), ce qui
// a masque le vrai comportement de la rangee. Redecode avec le bon seuil (640) : **toute la
// rangee y=35 (et y=36/37) sur CinnabarIsland_hns, de x=30 a x=48, est MB_OCEAN_WATER** -
// (38,35) etait donc TOUJOURS de l'eau malgre la passe precedente. Le Pokemon blesse est
// repositionne en (42,34), verifie MB_NORMAL/collision nulle par le meme decodage corrige, et
// atteignable depuis la position du joueur en restant entierement sur la bande seche y=34
// (x=38 a x=42, confirmee).
HeartSoul_Movement_VersPokemonBlesse:
	walk_down
	walk_right
	walk_right
	face_right
	step_end

// 2e correctif (meme cause : seuil de decodage errone dans la 9e passe, voir commentaire
// ci-dessus). Chemin recalcule pour rester sur la bande seche y=34 (jamais y=35, qui est de
// l'eau sur toute sa largeur) : simple repli vers l'interieur avant le fondu au noir. Le
// bateau lui-meme n'est plus mis en scene en exterieur (voir MAP_CINNABAR_ISLAND_BOAT_CABIN_HNS
// plus haut) : ce mouvement represente seulement quelques pas vers le quai, juste avant le
// fondu au noir/l'embarquement - la destination reelle (l'interieur du bateau) est atteinte
// par warp, pas par une correspondance visuelle exacte avec un quai dessine.
HeartSoul_Movement_VersPort:
	walk_left
	walk_left
	face_down
	step_end

// Scene de la tempete (MAP_CINNABAR_ISLAND_BOAT_CABIN_HNS) - marin en (8,3), joueur warpe en
// (8,7). Chemin verifie par decodage de data/layouts/CinnabarIsland_BoatCabin_hns/map.bin
// (copie de CinnabarIsland_Mansion_Hns : piece ouverte sur toute la colonne x=8, aucun
// obstacle sur ce trajet).
HeartSoul_Movement_MarinApproche:
	walk_down
	walk_down
	walk_down
	face_down
	step_end

// Reconciliation du document de design detaille : le GRUNT en fuite qui laisse tomber le
// carnet (evenement 2), traverse reellement l'ecran. Position de depart (44,30) et chemin
// verifies par le decodage corrige (voir commentaire au-dessus de HeartSoul_Movement_
// VersPokemonBlesse) : toute la rangee y=30 de x=37 a x=47 est seche (MB_NORMAL/MB_TALL_GRASS,
// collision nulle), une rangee au-dessus du joueur (qui se trouve alors vers y=31) - visible
// a l'ecran sans traverser son chemin.
HeartSoul_Movement_GruntFuite:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	face_left
	step_end

// Placeholders requis par le format object_event (scripts jamais reellement declenches : les
// objets sont verrouilles sous lockall pendant toute leur fenetre de visibilite, puis retires
// par removeobject).
CinnabarIsland_EventScript_GruntIntroUnused::
	end

CinnabarIsland_EventScript_Grunt2Unused::
	end

CinnabarIsland_EventScript_RefugeeUnused::
	end

CinnabarIsland_EventScript_RefugeeEnfantUnused::
	end

CinnabarIsland_EventScript_PokemonBlesseUnused::
	end

CinnabarIsland_EventScript_GruntFuiteUnused::
	end

HeartSoul_Text_AttaqueDebut:
	.string "Un grondement secoue Cinnabar.\p"
	.string "Des silhouettes en uniforme noir\n"
	.string "debarquent sur le port, la TEAM\l"
	.string "ROCKET.\p"
	.string "Les liaisons avec le reste de\n"
	.string "Kanto viennent d'etre coupees.$"

HeartSoul_Text_BlaineReagit:
	.string "BLAINE: Rocket… deja ?!$"

HeartSoul_Text_BlaineSeplaceDevant:
	.string "BLAINE se place entre le GRUNT\n"
	.string "et toi.\p"
	.string "BLAINE: Pars maintenant, je le\n"
	.string "retiens !$"

HeartSoul_Text_EclatCombat:
	.string "Un eclair jaillit derriere toi.\n"
	.string "Tu ne vois pas l'issue du\l"
	.string "combat.$"

HeartSoul_Text_BlaineDisparu:
	.string "… Le silence retombe. BLAINE a\n"
	.string "disparu dans la fumee.$"

// Evenement 0 (histoire.md section 11, Acte I) : obstacle mineur, pas un boss - le GRUNT que
// Blaine a retenu se retourne desormais contre le joueur, avant que celui-ci puisse continuer
// vers les refugies du Mont Cinnabar. TRAINER_ROCKET_GRUNT_CINNABAR_HNS (niveau 34, convention
// plate deja utilisee partout dans ce projet).
HeartSoul_Text_GruntSeen:
	.string "GRUNT: Tiens, une Championne.\n"
	.string "Ca tombe bien.$"

HeartSoul_Text_GruntBeaten:
	.string "GRUNT: …\p"
	.string "Pas cette fois.$"

HeartSoul_Text_GruntApresBattle:
	.string "Le GRUNT bat en retraite. Le\n"
	.string "passage vers le MONT CINNABAR\l"
	.string "est libre.$"

// Evenement 1 (histoire.md section 11, Acte I) : choix rapide, sans jugement moral appuye,
// entre securiser le Centre Pokemon (soigner les blesses) ou l'Arene (proteger son propre
// titre). +1 reputation si Centre Pokemon, 0 si Arene - barème documente dans histoire.md.
HeartSoul_Text_SauvetageIntro:
	.string "Dans le chaos, deux batiments\n"
	.string "ont encore besoin de toi avant\l"
	.string "de fuir : le CENTRE POKEMON, ou\l"
	.string "ta propre ARENE.$"

HeartSoul_Text_SauvetageCentre:
	.string "Securiser le CENTRE POKEMON$"

HeartSoul_Text_SauvetageArene:
	.string "Securiser ton ARENE$"

HeartSoul_Text_SauvetageCentreResultat:
	.string "Tu aides a evacuer les blesses du\n"
	.string "CENTRE POKEMON avant de\l"
	.string "repartir en courant.$"

HeartSoul_Text_SauvetageAreneResultat:
	.string "Tu recuperes en vitesse ce qui\n"
	.string "prouve ton titre de Championne,\l"
	.string "avant de repartir en courant.$"

// Evenement 2 (histoire.md section 11, Acte I) : objet de lore, prepare la quete
// d'interception radio de Carmin-sur-Mer (HeartSoul_EventScript_InterceptionRadio,
// heart_and_soul_act2.inc) - purement narratif, aucun flag necessaire.
HeartSoul_Text_CarnetGrunt:
	.string "Un GRUNT de la TEAM ROCKET te\n"
	.string "double en courant et laisse\l"
	.string "tomber un carnet couvert de\l"
	.string "codes radio griffonnes.\p"
	.string "Tu le ramasses sans vraiment\n"
	.string "savoir a quoi il sert. Pas le\l"
	.string "moment de t'attarder.$"

HeartSoul_Text_RefugiesIntro:
	.string "En te frayant un chemin vers le\n"
	.string "port, tu croises des habitants\l"
	.string "caches dans les grottes du\l"
	.string "MONT CINNABAR.\p"
	.string "Ils te supplient de decider pour\n"
	.string "eux.$"

HeartSoul_Text_RefugiesGuider:
	.string "Les guider vers un bateau$"

HeartSoul_Text_RefugiesCacher:
	.string "Les cacher sur place$"

HeartSoul_Text_RefugiesGuiderResultat:
	.string "Tu guides le petit groupe jusqu'a\n"
	.string "un bateau de fortune, au risque\l"
	.string "d'etre reperes par la TEAM\l"
	.string "ROCKET.\p"
	.string "Ils s'eloignent du port, vivants.$"

HeartSoul_Text_RefugiesCacherResultat:
	.string "Tu les caches du mieux que tu\n"
	.string "peux dans les grottes.\p"
	.string "Pas de garantie qu'ils soient\n"
	.string "toujours la a ton retour…$"

HeartSoul_Text_FuiteRoute21:
	.string "Il n'y a plus rien a faire ici.\p"
	.string "Cinnabar est coupee du monde. Un\n"
	.string "dernier bateau attend au port,\l"
	.string "pret a partir vers le continent.$"

HeartSoul_Text_ObjetTrouve:
	.string "En courant vers le port, tu\n"
	.string "trebuches sur un bracelet grave,\l"
	.string "tombe des affaires d'une famille\l"
	.string "de refugies.\p"
	.string "Tu le glisses dans ta poche. Il\n"
	.string "faudra le rendre, si tu croises\l"
	.string "quelqu'un qui le reconnait.$"

// Evenement 3 (histoire.md section 11, Acte I) : compagnon informel, paie en Acte V (mention
// ajoutee dans HeartSoul_EventScript_BlaineRetrouve, heart_and_soul_act4.inc, si
// FLAG_POKEMON_BLESSE_SAUVE est pose). Pas un membre d'equipe (aucun givemon), purement
// narratif + flag de suivi.
HeartSoul_Text_PokemonBlesseIntro:
	.string "Un POKEMON sauvage erre pres du\n"
	.string "port, blesse par les combats.\p"
	.string "Tu as le temps de l'aider, ou de\n"
	.string "continuer ta course.$"

HeartSoul_Text_PokemonBlesseSoigner:
	.string "L'aider$"

HeartSoul_Text_PokemonBlesseIgnorer:
	.string "Continuer ta course$"

HeartSoul_Text_PokemonBlesseSoignerResultat:
	.string "Tu prends un instant pour le\n"
	.string "soigner. Il te suit, mefiant,\l"
	.string "jusqu'au port.$"

HeartSoul_Text_PokemonBlesseIgnorerResultat:
	.string "Pas le temps. Tu le laisses\n"
	.string "derriere toi.$"

// Evenement 4 (histoire.md section 11, Acte II) : marin secondaire, reapparait en Acte V
// pour le retour par mer - purement narratif ici, FLAG_MARIN_ROUTE21_RENCONTRE pose pour un
// eventuel rappel ulterieur.
HeartSoul_Text_TempeteRoute21Intro:
	.string "La traversee est a peine\n"
	.string "commencee qu'une tempete\l"
	.string "soudaine s'abat sur la mer.$"

HeartSoul_Text_TempeteRoute21:
	.string "MARIN: Accroche-toi !\p"
	.string "Le marin te hurle de te\n"
	.string "cramponner et force l'accostage\l"
	.string "en urgence sur le continent.\p"
	.string "Tu ne connais meme pas son nom.\n"
	.string "Tu ne l'oublieras pas.$"

// Blocage physique du retour a Cinnabar (FLAG_CINNABAR_VERROUILLEE). Declenche depuis
// CinnabarIsland_hns_MapScripts (data/maps/CinnabarIsland_hns/scripts.inc), via
// MAP_SCRIPT_ON_FRAME_TABLE + map_script_2 (pas ON_TRANSITION : meme regle que le blocage
// deja documente sur CinnabarIsland_PokemonCenter_hns, un menu/msgbox interactif ne peut pas
// tourner avant que l'ecran soit pret). Renvoie le joueur vers le point d'arrivee verifie sur
// le continent (voir HeartSoul_CinnabarAttack_Verrouillage plus haut) plutot que de bloquer
// silencieusement son deplacement.
HeartSoul_EventScript_CinnabarVerrouilleeCheck::
	goto_if_unset FLAG_CINNABAR_VERROUILLEE, HeartSoul_CinnabarVerrouillee_Skip
	goto_if_set FLAG_ACTE_5_DEBLOQUE, HeartSoul_CinnabarVerrouillee_Skip
	lockall
	msgbox HeartSoul_Text_CinnabarToujoursVerrouillee, MSGBOX_DEFAULT
	closemessage
	fadescreenswapbuffers FADE_TO_BLACK
	warp MAP_PALLET_TOWN_HNS, 6, 8
	waitstate
	setvar VAR_TEMP_0, 1
	end
HeartSoul_CinnabarVerrouillee_Skip:
	@ Sans ce setvar, la condition VAR_TEMP_0==0 de CinnabarIsland_hns_MapScripts (ON_FRAME_TABLE)
	@ reste vraie a chaque frame : TryRunOnFrameMapScript() (src/script.c) renvoie alors TRUE en
	@ boucle et ProcessPlayerFieldInput() (src/field_control_avatar.c) ne traite plus jamais le
	@ deplacement ni le bouton START tant que le joueur reste sur cette carte - vu comme un
	@ freeze total. Meme motif que CeruleanCity_Gym_EventScript_TryMachinePart (qui, lui, pose
	@ bien ce setvar sur chaque sortie) : VAR_TEMP_0 est remis a 0 par le moteur a chaque nouveau
	@ chargement de carte (ClearTempFieldEventData, src/event_data.c), donc ce setvar ne fait que
	@ desarmer la verification avant le prochain chargement, sans changer son resultat.
	setvar VAR_TEMP_0, 1
	end

HeartSoul_Text_CinnabarToujoursVerrouillee:
	.string "La TEAM ROCKET tient toujours le\n"
	.string "port. Impossible d'accoster ici\l"
	.string "pour l'instant.$"
