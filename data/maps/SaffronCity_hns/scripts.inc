SaffronCity_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SaffronCity_EventScript_CityEnter
	.byte 0

SaffronCity_OnFrame::
	map_script_2 VAR_SAFFRON_CITY_STATE, 0,  SaffronCity_EventScript_BaobaCall
	.2byte 0

SaffronCity_EventScript_CityEnter::
	setflag FLAG_VISITED_SAFFRON_CITY
	end

SaffronCity_EventScript_BaobaCall::
	lock
	setspeaker SaffronCity_Text_BaobaName
	pokenavcall SaffronCity_Text_Baoba
	waitmessage
	delay 30
	setflag FLAG_SAFARI_ZONE_EAST_EXPANSION
	setvar VAR_SAFFRON_CITY_STATE, 1
	release
	end

SaffronCity_Text_BaobaName:
	.string "BAOBA$"

SaffronCity_Text_Baoba:
	.string "GARDE BAOBA: {PLAYER}!\p"
	.string "Ici le GARDE BAOBA.\p"
	.string "Je t'appelle pour te dire que\n"
	.string "la SAFARI ZONE s'est encore\l"
	.string "agrandie!\p"
	.string "Ma SAFARI ZONE est enfin\n"
	.string "pleinement réalisée!\p"
	.string "Tu te souviens où c'est, non?\p"
	.string "Juste à l'ouest de CIANWOOD\n"
	.string "CITY! À bientôt!$"

SaffronCity_EventScript_Lass1A::
	goto_if_set FLAG_RETURNED_MACHINE_PART, SaffronCity_EventScript_Lass1B
	msgbox SaffronCity_Text_Lass1, MSGBOX_NPC
	end

SaffronCity_EventScript_Lass1B::
	msgbox SaffronCity_Text_Lass1_ReturnedPart, MSGBOX_NPC
	end

SaffronCity_EventScript_Lass2::
	msgbox SaffronCity_Text_Lass2, MSGBOX_NPC
	end

SaffronCity_EventScript_PokefanM::
	goto_if_set FLAG_RETURNED_MACHINE_PART, SaffronCity_EventScript_PokefanM2
	msgbox SaffronCity_Text_PokefanM, MSGBOX_NPC
	end

SaffronCity_EventScript_PokefanM2::
	msgbox SaffronCity_Text_PokefanM_ReturnedPart, MSGBOX_NPC
	end

SaffronCity_EventScript_CooltrainerM::
	msgbox SaffronCity_Text_CooltrainerM, MSGBOX_NPC
	end

SaffronCity_EventScript_CooltrainerF::
	msgbox SaffronCity_Text_CooltrainerF, MSGBOX_NPC
	end

SaffronCity_EventScript_Fisher::
	goto_if_set FLAG_RETURNED_MACHINE_PART, SaffronCity_EventScript_Fisher2
	msgbox SaffronCity_Text_Fisher, MSGBOX_NPC
	end

SaffronCity_EventScript_Fisher2::
	msgbox SaffronCity_Text_Fisher_ReturnedPart, MSGBOX_NPC
	end

SaffronCity_EventScript_Youngster1::
	msgbox SaffronCity_Text_Youngster1, MSGBOX_NPC
	end

SaffronCity_EventScript_Youngster2::
	msgbox SaffronCity_Text_Youngster2, MSGBOX_NPC
	end
SaffronCity_EventScript_CitySign::
	msgbox SaffronCity_Text_Sign, MSGBOX_SIGN
	end

SaffronCity_EventScript_GymSign::
	msgbox SaffronCity_Text_GymSign, MSGBOX_SIGN
	end

SaffronCity_EventScript_DojoSign::
	msgbox SaffronCity_Text_FightingDojoSign, MSGBOX_SIGN
	end

SaffronCity_EventScript_SilphCoSign::
	msgbox SaffronCity_Text_SilphCoSign, MSGBOX_SIGN
	end

SaffronCity_EventScript_MrPsychicSign::
	msgbox SaffronCity_Text_MrPsychicHouseSign, MSGBOX_SIGN
	end

SaffronCity_EventScript_MagnetTrainSign::
	msgbox SaffronCity_Text_MagnetTrainStationSign, MSGBOX_SIGN
	end

SaffronCity_EventScript_Elli::
	msgbox SaffronCity_Text_Elli, MSGBOX_NPC
	end

SaffronCity_Text_Elli:
	.string "J'adore VULPIX! :D$"

SaffronCity_Text_Lass1:
	.string "Une petite fille très douée\n"
	.string "pour imiter les gens habite ici.\p"
	.string "Elle imite même ceux à qui\n"
	.string "elle parle. C'est déroutant.$"

SaffronCity_Text_Lass1_ReturnedPart:
	.string "La fille COPYCAT avait l'air\n"
	.string "triste.\p"
	.string "Elle a dit avoir perdu sa POKé\n"
	.string "DOLL CLEFAIRY préférée.$"

SaffronCity_Text_PokefanM:
	.string "Tu viens de JOHTO?\p"
	.string "Tu peux rentrer vite fait si le\n"
	.string "MAGNET TRAIN fonctionne.$"

SaffronCity_Text_PokefanM_ReturnedPart:
	.string "Tu viens de JOHTO?\p"
	.string "Tu peux rentrer vite fait en\n"
	.string "prenant le MAGNET TRAIN.$"

SaffronCity_Text_CooltrainerM:
	.string "Je suis allé à l'ARENE, prêt à\n"
	.string "en découdre avec d'autres\l"
	.string "dresseurs…\p"
	.string "En fait, je me suis trompé et\n"
	.string "je suis tombé sur l'ARENE\l"
	.string "désaffectée d'à côté.\p"
	.string "Ah là là, j'étais plutôt\n"
	.string "gêné.$"

SaffronCity_Text_CooltrainerF:
	.string "Voici SILPH CO., célèbre pour\n"
	.string "ses produits dérivés POKéMON.\p"
	.string "Autrefois, la TEAM ROCKET\n"
	.string "convoitait l'entreprise pour\l"
	.string "cette raison.$"

SaffronCity_Text_Fisher:
	.string "Mâche… mâche…\p"
	.string "On dit qu'il y a de gros\n"
	.string "ennuis qui couvent à la\l"
	.string "POWER PLANT.\p"
	.string "Mâche… mâche…$"

SaffronCity_Text_Fisher_ReturnedPart:
	.string "Mâche… mâche…\p"
	.string "On m'a dit qu'il y a eu de\n"
	.string "gros ennuis à la POWER\l"
	.string "PLANT.\p"
	.string "Mâche… mâche… Aaah, je suis\n"
	.string "rassasié!$"

SaffronCity_Text_Youngster1:
	.string "M'aventurer dans une ruelle\n"
	.string "pour la première fois me rend\l"
	.string "un peu anxieux.$"

SaffronCity_Text_Youngster2:
	.string "Il existe un endroit appelé\n"
	.string "TRAINER HOUSE à VIRIDIAN où\l"
	.string "des dresseurs se rassemblent\l"
	.string "de partout.$"

SaffronCity_Text_Lass2:
	.string "Notre ville est passée à la\n"
	.string "radio.\p"
	.string "C'est agréable d'entendre du\n"
	.string "bien de sa ville, mais c'est\l"
	.string "aussi un peu gênant.$"

SaffronCity_Text_Sign:
	.string "SAFFRON CITY\p"
	.string "La Brillante Cité Dorée du\n"
	.string "Commerce$"

SaffronCity_Text_GymSign:
	.string "SAFFRON CITY\n"
	.string "ARENE POKéMON\l"
	.string "CHAMPION: SABRINA\p"
	.string "La Maîtresse des POKéMON\n"
	.string "psychiques!$"

SaffronCity_Text_FightingDojoSign:
	.string "Tout le monde est bienvenu!\n"
	.string "DOJO DE COMBAT$"

SaffronCity_Text_SilphCoSign:
	.string "SILPH CO.\n"
	.string "IMMEUBLE DE BUREAUX$"

SaffronCity_Text_MrPsychicHouseSign:
	.string "MAISON DE\n"
	.string "MR.PSYCHIC$"

SaffronCity_Text_MagnetTrainStationSign:
	.string "SAFFRON CITY\n"
	.string "STATION DU MAGNET TRAIN$"
