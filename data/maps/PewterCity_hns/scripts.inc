PewterCity_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PewterCity_EventScript_CityEnter
	.byte 0

PewterCity_EventScript_CityEnter::
	setflag FLAG_VISITED_PEWTER_CITY
	end

PewterCity_EventScript_CooltrainerF::
	msgbox PewterCity_Text_CooltrainerF, MSGBOX_NPC
	end

PewterCity_EventScript_BugCatcher::
	msgbox PewterCity_Text_BugCatcher, MSGBOX_NPC
	end

PewterCity_EventScript_Gramps::
	lock
	faceplayer
	goto_if_eq VAR_LUGIA_OR_HOOH, 1, PewterCity_EventScript_GrampsRainbowWing
	goto_if_eq VAR_LUGIA_OR_HOOH, 2, PewterCity_EventScript_GrampsSilverWing
	end

PewterCity_EventScript_GrampsRainbowWing::
	checkitem ITEM_RAINBOW_WING
	goto_if_eq VAR_RESULT, TRUE, PewterCity_EventScript_GrampsDone
	msgbox PewterCity_Text_Gramps, MSGBOX_DEFAULT
	setvar VAR_COMPLETED_HO_OH, 4
	giveitem ITEM_RAINBOW_WING
	setflag FLAG_HIDE_TINTOWER_GUARD
	closemessage
	release
	end

PewterCity_EventScript_GrampsSilverWing::
	checkitem ITEM_SILVER_WING
	goto_if_eq VAR_RESULT, TRUE, PewterCity_EventScript_GrampsDone
	msgbox PewterCity_Text_Gramps, MSGBOX_DEFAULT
	setvar VAR_COMPLETED_LUGIA, 4
	giveitem ITEM_SILVER_WING
	setflag FLAG_HIDE_WHIRL_ISLANDS_TENTACRUEL
	closemessage
	release
	end

PewterCity_EventScript_GrampsDone::
	msgbox PewterCity_Text_Gramps_GotSilverWing, MSGBOX_DEFAULT
	closemessage
	release
	end

PewterCity_EventScript_Bugcatcher::
	msgbox PewterCity_Text_BugCatcher, MSGBOX_NPC
	end

PewterCity_EventScript_Cooltrainer::
	msgbox PewterCity_Text_CooltrainerF, MSGBOX_NPC
	end

PewterCity_Text_CooltrainerF:
	.string "As-tu visité la PEWTER GYM ?\p"
	.string "Le CHEF utilise des POKéMON de\n"
	.string "type ROCHE.$"

PewterCity_Text_BugCatcher:
	.string "La nuit, les CLEFAIRY viennent\n"
	.string "jouer au MT.MOON.\p"
	.string "Mais pas toutes les nuits.$"

PewterCity_Text_Gramps:
	.string "Ah, tu es venu de si loin,\n"
	.string "depuis JOHTO ?\p"
	.string "Ça me rappelle des souvenirs.\n"
	.string "Quand j'étais jeune, je suis allé\l"
	.string "m'entraîner à JOHTO.\p"
	.string "Tu me rappelles tellement ce\n"
	.string "que j'étais, jeune homme.\p"
	.string "Tiens. Je veux que tu aies cet\n"
	.string "objet que j'ai trouvé à JOHTO.$"

PewterCity_Text_Gramps_GotSilverWing:
	.string "Découvrir de nouveaux endroits\n"
	.string "et rencontrer de nouvelles\l"
	.string "personnes…\p"
	.string "Voilà les joies du voyage.$"

PewterCity_EventScript_CitySign::
	msgbox PewterCity_Text_Sign, MSGBOX_NPC
	end

PewterCity_EventScript_GymSign::
	msgbox PewterCity_Text_GymSign, MSGBOX_NPC
	end

PewterCity_EventScript_MuseumSign::
	msgbox PewterCity_Text_MuseumSign, MSGBOX_NPC
	end

PewterCity_EventScript_MtMoonSign::
	msgbox PewterCity_Text_MtMoonGiftShopSign, MSGBOX_NPC
	end

PewterCity_EventScript_WelcomeSign::
	msgbox PewterCity_Text_WelcomeSign, MSGBOX_NPC
	end

PewterCity_Text_Sign:
	.string "PEWTER CITY\n"
	.string "Une ville gris pierre$"

PewterCity_Text_GymSign:
	.string "PEWTER CITY POKéMON GYM\n"
	.string "CHEF : PIERRE\p"
	.string "Le dresseur POKéMON solide comme\n"
	.string "la roche$"

PewterCity_Text_MuseumSign:
	.string "Il y a un avis ici…\p"
	.string "Le PEWTER MUSEUM OF SCIENCE est\n"
	.string "fermé pour rénovations…$"

PewterCity_Text_MtMoonGiftShopSign:
	.string "Il y a un avis ici…\p"
	.string "MT.MOON GIFT SHOP MAINTENANT\n"
	.string "OUVERTE !$"

PewterCity_Text_WelcomeSign:
	.string "BIENVENUE À PEWTER CITY !$"

