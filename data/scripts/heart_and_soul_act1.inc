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
HeartSoul_EventScript_CinnabarAttack::
	lockall
	setflag FLAG_ATTAQUE_CINNABAR_LANCEE
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
// motif deja eprouve avec Blaine). Un court delay est egalement ajoute avant le premier
// applymovement : par prudence, le temps que le rendu des object_events se stabilise
// completement apres un warp scripte (aucune preuve definitive que cela ait contribue au bug
// signale - Blaine n'a jamais eu son flag touche par ce chantier - mais mitigation standard,
// peu couteuse, pour ce type de probleme de timing juste apres une transition de carte).
HeartSoul_EventScript_CinnabarAttackPart2::
	lockall
	delay 20
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
	msgbox HeartSoul_Text_EclatCombat, MSGBOX_DEFAULT
	fadescreenswapbuffers FADE_TO_BLACK
	setflag FLAG_HIDE_CINNABAR_BLAINE
	removeobject LOCALID_CINNABAR_BLAINE
	setflag FLAG_BLAINE_DISPARU
	fadescreenswapbuffers FADE_FROM_BLACK
	msgbox HeartSoul_Text_BlaineDisparu, MSGBOX_DEFAULT
	applymovement LOCALID_CINNABAR_GRUNT_INTRO, HeartSoul_Movement_GruntSeRetourne
	waitmovement 0
	trainerbattle_single TRAINER_ROCKET_GRUNT_CINNABAR_HNS, HeartSoul_Text_GruntSeen, HeartSoul_Text_GruntBeaten
	msgbox HeartSoul_Text_GruntApresBattle, MSGBOX_DEFAULT
	setflag FLAG_HIDE_CINNABAR_GRUNT_INTRO
	removeobject LOCALID_CINNABAR_GRUNT_INTRO
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
HeartSoul_CinnabarAttack_CarnetGrunt:
	msgbox HeartSoul_Text_CarnetGrunt, MSGBOX_DEFAULT
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
	setflag FLAG_CINNABAR_VERROUILLEE
	setflag FLAG_ACTE_1_TERMINE
	setflag FLAG_OBJET_PERDU_TROUVE
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
	msgbox HeartSoul_Text_ObjetTrouve, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_FuiteRoute21, MSGBOX_DEFAULT
	setflag FLAG_MARIN_ROUTE21_RENCONTRE
	msgbox HeartSoul_Text_TempeteRoute21, MSGBOX_DEFAULT
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

HeartSoul_Movement_GruntSeRetourne:
	face_down
	step_end

// Placeholder requis par le format object_event (script jamais reellement declenche : l'objet
// est verrouille sous lockall pendant toute la fenetre ou il est visible, puis retire par
// removeobject des la fin du combat).
CinnabarIsland_EventScript_GruntIntroUnused::
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
HeartSoul_Text_TempeteRoute21:
	.string "La traversee est interrompue par\n"
	.string "une tempete soudaine.\p"
	.string "Un marin te hurle de te\n"
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
