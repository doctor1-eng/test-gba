ViridianCity_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ViridianCity_EventScript_CityEnter
	map_script MAP_SCRIPT_ON_FRAME_TABLE, ViridianCity_OnFrame
	.byte 0

ViridianCity_EventScript_CityEnter::
	setflag FLAG_VISITED_VIRIDIAN_CITY
	end

ViridianCity_OnFrame::
	map_script_2 VAR_OAK_MT_SILVER_CALL, 1, ViridianCity_EventScript_OakCall
	.2byte 0

ViridianCity_EventScript_OakCall::
	lock
	setspeaker ViridianCity_Text_OakName
	pokenavcall ViridianCity_Text_OakCall
	waitmessage
	delay 30
	setvar VAR_OAK_MT_SILVER_CALL, 2
	release
	end

ViridianCity_Text_OakName:
	.string "OAK$"

ViridianCity_Text_OakCall:
	.string "OAK: Ah, {PLAYER} !\p"
	.string "Passe me voir à mon\n"
	.string "LABORATOIRE de PALLET TOWN\l"
	.string "quand tu en auras l'occasion.$"

ViridianCity_EventScript_Gramps::
	lock
	faceplayer
	msgbox ViridianCity_Text_CoffeeGramps_Question, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, FALSE, ViridianCity_EventScript_GrampsDoubted
	msgbox ViridianCity_Text_CoffeeGramps_Believed, MSGBOX_DEFAULT
	closemessage
	release
	end

ViridianCity_EventScript_GrampsDoubted::
	msgbox ViridianCity_Text_CoffeeGramps_Doubted, MSGBOX_DEFAULT
	closemessage
	release
	end

VirianCity_EventScript_GymGramps::
	goto_if_set FLAG_HIDE_CINNABAR_BLUE, VirianCity_EventScript_GymGrampsBlue
	msgbox ViridianCity_Text_GrampsNearGym, MSGBOX_NPC
	end

VirianCity_EventScript_GymGrampsBlue::
	msgbox ViridianCity_Text_GrampsNearGym_BlueReturned, MSGBOX_NPC
	end

ViridianCity_EventScript_Fisher::
	goto_if_set FLAG_GOT_VIRIDIAN_TM_DREAM_EATER, ViridianCity_EventScript_FisherAfter
	msgbox ViridianCity_Text_DreamEaterFisher, MSGBOX_NPC
	giveitem ITEM_TM_DREAM_EATER
	setflag FLAG_GOT_VIRIDIAN_TM_DREAM_EATER
	end

ViridianCity_EventScript_FisherAfter::
	msgbox ViridianCity_Text_DreamEaterFisher_GotTM, MSGBOX_NPC
	end

ViridianCity_EventScript_Youngster::
	msgbox ViridianCity_Text_Youngster, MSGBOX_NPC
	end

ViridianCity_EventScript_CitySign::
	msgbox ViridianCity_Text_WelcomeSign, MSGBOX_SIGN
	end

ViridianCity_EventScript_CitySign2::
	msgbox ViridianCity_Text_Sign, MSGBOX_SIGN
	end

ViridianCity_EventScript_GymSign::
	msgbox ViridianCity_Text_GymSign, MSGBOX_SIGN
	end

ViridianCity_Text_CoffeeGramps_Question:
	.string "Hé, gamin ! Je viens de prendre\n"
	.string "un double expresso, je suis à\l"
	.string "fond !\p"
	.string "J'ai besoin de parler à quelqu'un,\n"
	.string "alors tu feras l'affaire !\p"
	.string "Je n'ai peut-être pas l'air comme\n"
	.string "ça, mais j'étais un expert pour\l"
	.string "attraper les POKéMON.\p"
	.string "Tu me crois ?$"

ViridianCity_Text_CoffeeGramps_Believed:
	.string "Bien, bien. Oui, j'étais quelqu'un\n"
	.string "d'exceptionnel, crois-moi\l"
	.string "sur parole !$"

ViridianCity_Text_CoffeeGramps_Doubted:
	.string "Quoi ? Petit morveux !\p"
	.string "Si j'étais juste un peu plus\n"
	.string "jeune, je te montrerais deux ou\l"
	.string "trois choses. Humph !$"

ViridianCity_Text_GrampsNearGym:
	.string "Cette ARENE n'avait pas de\n"
	.string "Champion jusqu'à récemment.\p"
	.string "Un jeune homme de PALLET est\n"
	.string "devenu Champion, mais il\l"
	.string "s'absente souvent.$"

ViridianCity_Text_GrampsNearGym_BlueReturned:
	.string "Tu vas affronter le\n"
	.string "Champion ?\p"
	.string "Bonne chance, tu vas en avoir\n"
	.string "besoin.$"

ViridianCity_Text_DreamEaterFisherNew:
	.string "…Zzzzz…\p"
	.string "Baaah !\p"
	.string "J'ai dû m'assoupir au soleil.\p"
	.string "Quoi ? Je te rappelle quelqu'un ?\p"
	.string "Je sais pas, tiens, prends ça.$"


ViridianCity_Text_DreamEaterFisher_GotTM::
	.string "…Zzzzz…$"

ViridianCity_Text_DreamEaterFisher:
	.string "Baaah !\p"
	.string "J'ai dû m'assoupir au soleil.\p"
	.string "…J'ai rêvé qu'un DROWZEE\n"
	.string "mangeait mon rêve. Bizarre, non ?\p"
	.string "Hein ? Qu'est-ce que c'est ?\p"
	.string "D'où vient cette TM ?\p"
	.string "C'est flippant ! Tiens, tu peux\n"
	.string "garder cette TM.$"

ViridianCity_Text_DreamEaterFisher_GotDreamEater:
	.string "La TM42 contient DREAM EATER…\p"
	.string "…Zzzzz…$"

ViridianCity_Text_Youngster:
	.string "J'ai entendu dire qu'il y a plein\n"
	.string "d'objets au sol dans VIRIDIAN\l"
	.string "FOREST.$"

ViridianCity_Text_Sign:
	.string "VIRIDIAN CITY\p"
	.string "Le Paradis Éternellement Vert$"

ViridianCity_Text_GymSign:
	.string "ARENE POKéMON DE VIRIDIAN CITY\n"
	.string "CHAMPION : …\p"
	.string "Le reste du texte est illisible…$"

ViridianCity_Text_WelcomeSign:
	.string "BIENVENUE À VIRIDIAN CITY,\p"
	.string "LA PORTE D'ENTRÉE VERS INDIGO PLATEAU$"

ViridianCity_Text_TrainerHouseSign:
	.string "TRAINER HOUSE\p"
	.string "Le Club des Combats de\n"
	.string "Dresseurs d'Élite$"
