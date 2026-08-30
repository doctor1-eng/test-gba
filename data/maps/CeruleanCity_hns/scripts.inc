CeruleanCity_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CeruleanCity_EventScript_CityEnter
	.byte 0

CeruleanCity_EventScript_CityEnter::
	setflag FLAG_VISITED_CERULEAN_CITY
	end

CeruleanCity_EventScript_CooltrainerM1::
	goto_if_set FLAG_RETURNED_MACHINE_PART, CeruleanCity_EventScript_CooltrainerMReturnedPart
	msgbox CeruleanCity_Text_CooltrainerM1, MSGBOX_NPC
	end

CeruleanCity_EventScript_CooltrainerMReturnedPart::
	msgbox CeruleanCity_Text_CooltrainerM2, MSGBOX_NPC
	end

CeruleanCity_EventScript_Boy::
	goto_if_unset FLAG_ACTE_1_TERMINE, CeruleanCity_EventScript_BoyFlavor
	goto_if_set FLAG_CANALISATIONS_REPAREES, CeruleanCity_EventScript_BoyAfter
	lock
	faceplayer
	call HeartSoul_EventScript_CanalisationsCerulean
	release
	end

CeruleanCity_EventScript_BoyFlavor::
	msgbox CeruleanCity_Text_SuperNerd, MSGBOX_NPC
	end

CeruleanCity_EventScript_BoyAfter::
	msgbox HeartSoul_Text_CanalisationsApres, MSGBOX_NPC
	end

CeruleanCity_EventScript_Guard::
	msgbox CeruleanCity_Text_ThisIsCeruleanCave, MSGBOX_NPC
	end

CeruleanCity_Text_ThisIsCeruleanCave::
    .string "Ici, c'est la CERULEAN CAVE.\p"
    .string "Des POKéMON terriblement puissants\n"
    .string "y vivent.\p"
    .string "Il faut être un DRESSEUR très\n"
    .string "spécial pour avoir le droit d'y\l"
    .string "entrer.\p"
    .string "Il faudrait déjà être assez fort\n"
    .string "pour devenir CHAMPION de la LIGUE\l"
    .string "POKéMON de KANTO, pour commencer.\p"
    .string "Et il faudrait aussi avoir accompli\n"
    .string "un exploit remarquable.$"

CeruleanCity_Text_CooltrainerM1:
	.string "La CENTRALE ÉLECTRIQUE de KANTO ?\p"
	.string "Elle est près de la fin de la\n"
	.string "ROUTE 9, la route qui part vers\l"
	.string "l'est d'ici.\p"
	.string "Je crois qu'il y a eu une sorte\n"
	.string "d'accident là-bas.$"

CeruleanCity_Text_CooltrainerM2:
	.string "Tu essaies de collectionner tous\n"
	.string "les POKéMON ?\p"
	.string "Ça doit être tout un défi, mais\n"
	.string "ça a l'air amusant aussi.$"

CeruleanCity_Text_SuperNerd:
	.string "Le CAPE au nord est un bon endroit\n"
	.string "pour un rencard.\p"
	.string "Les filles adorent ça !$"

CeruleanCity_EventScript_Slowbro::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_SLOWBRO, CRY_MODE_NORMAL
	msgbox CeruleanCity_Text_Slowbro, MSGBOX_DEFAULT
	waitmoncry
	release
	end

CeruleanCity_Text_Slowbro:
	.string "SLOWBRO: Yarah?$"

CeruleanCity_EventScript_SlowbroWoman::
	msgbox CeruleanCity_Text_CooltrainerF1, MSGBOX_NPC
	turnobject LOCALID_CERULEAN_CITY_WOMAN, DIR_EAST
	msgbox CeruleanCity_Text_CooltrainerF2, MSGBOX_DEFAULT
	delay 20
	playmoncry SPECIES_SLOWBRO, CRY_MODE_NORMAL
	applymovement2 LOCALID_CERULEAN_CITY_SLOWBRO, Common_Movement_QuestionMark
	turnobject LOCALID_CERULEAN_CITY_SLOWBRO DIR_WEST
	delay 30
	turnobject LOCALID_CERULEAN_CITY_SLOWBRO, DIR_EAST
	delay 30
	turnobject LOCALID_CERULEAN_CITY_SLOWBRO, DIR_SOUTH
	delay 70
	msgbox CeruleanCity_Text_CooltrainerF3, MSGBOX_NPC
	end

CeruleanCity_Text_CooltrainerF1:
	.string "Mon SLOWBRO et moi, on forme une\n"
	.string "équipe géniale !$"

CeruleanCity_Text_CooltrainerF2:
	.string "SLOWBRO, montre-moi ta CONFUSION !$"

CeruleanCity_Text_CooltrainerF3:
	.string "…$"

CeruleanCity_EventScript_Fisher::
	goto_if_eq VAR_CERULEAN_CITY_STATE, 2, CeruleanCity_EventScript_FisherDuringRocket
	msgbox CeruleanCity_Text_Fisher, MSGBOX_NPC
	end

CeruleanCity_EventScript_FisherDuringRocket::
	msgbox CeruleanCity_Text_Fisher_RocketTip, MSGBOX_NPC
	end

CeruleanCity_Text_Fisher:
	.string "Je suis un grand fan d'ONDINE,\n"
	.string "la championne de CERULEAN GYM.$"

CeruleanCity_Text_Fisher_RocketTip:
	.string "J'ai vu un type louche partir\n"
	.string "vers le CAPE de CERULEAN.$"

CeruleanCity_EventScript_Youngster::
	msgbox CeruleanCity_Text_Youngster1, MSGBOX_NPC
	end

CeruleanCity_Text_Youngster1:
	.string "La grotte au nord de la ville\n"
	.string "abrite des POKéMON redoutables.$"

CeruleanCity_Text_Youngster2:
	.string "Ah ouais ?\p"
	.string "Mon ITEMFINDER réagit…$"

CeruleanCity_EventScript_Sign::
	msgbox CeruleanCity_Text_Sign, MSGBOX_SIGN
	end

CeruleanCity_EventScript_GymSign::
	msgbox CeruleanCity_Text_GymSign, MSGBOX_SIGN
	end

CeruleanCity_EventScript_BikeShopSign::
	msgbox CeruleanCity_Text_BikeShopSign, MSGBOX_SIGN
	end

CeruleanCity_EventScript_PoliceSign::
	msgbox CeruleanCity_Text_PoliceSign, MSGBOX_SIGN
	end

CeruleanCity_EventScript_CapeSign::
	msgbox CeruleanCity_Text_CapeSign, MSGBOX_SIGN
	end

CeruleanCity_Text_Sign:
	.string "CERULEAN CITY\p"
	.string "Une mystérieuse aura bleue\n"
	.string "l'entoure$"

CeruleanCity_Text_GymSign:
	.string "CERULEAN CITY\n"
	.string "ARÈNE POKéMON\n"
	.string "CHAMPIONNE : ONDINE\p"
	.string "La Sirène Garçon Manqué$"

CeruleanCity_Text_BikeShopSign:
	.string "Il y a un avis affiché ici…\p"
	.string "Le MAGASIN DE VÉLOS a déménagé\n"
	.string "à GOLDENROD CITY, à JOHTO…$"

CeruleanCity_Text_PoliceSign:
	.string "Il y a un avis affiché ici…\p"
	.string "Luttons contre le vol et faisons\n"
	.string "de cette ville un endroit plus\l"
	.string "accueillant et joyeux !\p"
	.string "POLICE DE CERULEAN$"

CeruleanCity_Text_CapeSign:
	.string "CERULEAN CAPE\n"
	.string "DEVANT$"

CeruleanCity_Text_LockedDoor:
	.string "C'est verrouillé…$"

