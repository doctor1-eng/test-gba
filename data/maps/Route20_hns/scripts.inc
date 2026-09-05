Route20_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route20_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	.byte 0

Route20_EventScript_ShellBell::
	finditem ITEM_SHELL_BELL
	end

Route20_OnTransition:
	callnative SetTimeBasedEncounters
	end

Route20_EventScript_Nicole::
	trainerbattle_single TRAINER_NICOLE_HNS, Route20_Text_SwimmerfNicoleSeen, Route20_Text_SwimmerfNicoleBeaten, Route20_EventScript_RegisterNicole
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route20_EventScript_RematchNicole
	msgbox Route20_Text_SwimmerfNicoleAfterBattle, MSGBOX_AUTOCLOSE
	end

Route20_EventScript_RegisterNicole::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox Route20_Text_NicoleRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_NICOLE_HNS
	release
	end

Route20_EventScript_RematchNicole::
	trainerbattle_rematch TRAINER_NICOLE_HNS, Route20_Text_NicoleRematchIntro, Route20_Text_NicoleRematchDefeat
	msgbox Route20_Text_NicolePostRematch, MSGBOX_AUTOCLOSE
	goto_if_defeated TRAINER_NICOLE_2_HNS, Route20_EventScript_RematchNicoleTryGiveItem
	end

Route20_EventScript_RematchNicoleTryGiveItem::
	checkitem ITEM_HEART_SCALE
	goto_if_eq VAR_RESULT, TRUE, Common_EventScript_NoOp
	giveitem ITEM_HEART_SCALE
	closemessage
	end

Route20_EventScript_Lori::
	trainerbattle_single TRAINER_LORI_HNS, Route20_Text_SwimmerfLoriSeen, Route20_Text_SwimmerfLoriBeaten
	msgbox Route20_Text_SwimmerfLoriAfterBattle, MSGBOX_AUTOCLOSE
	end	

Route20_EventScript_Cameron::
	trainerbattle_single TRAINER_CAMERON_HNS, Route20_Text_SwimmermCameronSeen, Route20_Text_SwimmermCameronBeaten
	msgbox Route20_Text_SwimmermCameronAfterBattle, MSGBOX_AUTOCLOSE
	end	

Route20_EventScript_SeafoamSign::
	msgbox Route20_Text_SeafoamSign, MSGBOX_SIGN
	end

Route20_EventScript_GymSign::
	msgbox Route20_Text_CinnabarIsland_GymUnusedSign, MSGBOX_SIGN
	end

Route20_EventScript_Seel::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_SEEL, CRY_MODE_NORMAL
	msgbox Route20_Text_Seel, MSGBOX_DEFAULT
	waitmoncry
	release
	end

Route20_EventScript_Dewgong::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_DEWGONG, CRY_MODE_NORMAL
	msgbox Route20_Text_Dewgong, MSGBOX_DEFAULT
	waitmoncry
	release
	end

Route20_EventScript_Dennis::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_MAGIKARP, CRY_MODE_NORMAL
	msgbox Route20_Text_Dennis, MSGBOX_DEFAULT
	waitmoncry
	release
	end

Route20_EventScript_Headlocker::
	lock
	faceplayer
	msgbox Route20_Text_Headlocker, MSGBOX_DEFAULT
	closemessage
	turnobject LOCALID_ROUTE20_HEADLOCKER, DIR_SOUTH
	release
	end

Route20_Text_Dennis:
	.string "DENNIS: SHAAAAAAAA !$"

Route20_Text_Headlocker:
	.string "Il y a un POKéMON rare sur\n"
	.string "cette route.\p"
	.string "Et parfois, il porte un objet\n"
	.string "d'évolution rare.\p"
	.string "Ça fait deux fois plus rare !\p"
	.string "Je dois compléter mon POKéDEX,\n"
	.string "alors je pêche ici depuis des\l"
	.string "heures !$"

Route20_Text_Seel:
	.string "SEEL: Seel seel !$"

Route20_Text_Dewgong:
	.string "DEWGONG: Goooong !$"

Route20_Text_SeafoamSign:
	.string "SEAFOAM ISLANDS$"

Route20_Text_NicoleRegister:
	.string "Ton POKéMON nage vite !\n"
	.string "Laisse-moi prendre ton numéro !$"

Route20_Text_NicoleRematchIntro:
	.string "Toute cette nage a rendu mon\n"
	.string "POKéMON plus fort ! Battons-nous !$"

Route20_Text_NicoleRematchDefeat:
	.string "Encore emportée…$"

Route20_Text_NicolePostRematch:
	.string "Que je gagne ou perde, je suis en\n"
	.string "forme, et mes POKéMON aussi !$"

Route20_Text_SwimmerfNicoleSeen:
	.string "Je me sens tellement plus légère\n"
	.string "dans l'eau.$"

Route20_Text_SwimmerfNicoleBeaten:
	.string "Oh non !$"

Route20_Text_SwimmerfNicoleAfterBattle:
	.string "La nage fait travailler tout le\n"
	.string "corps.\p"
	.string "C'est vraiment bon pour la santé.$"

Route20_Text_SwimmerfLoriSeen:
	.string "Quelle impressionnante\n"
	.string "collection de BADGES. On se bat ?$"

Route20_Text_SwimmerfLoriBeaten:
	.string "Non !$"

Route20_Text_SwimmerfLoriAfterBattle:
	.string "SURF n'est plus la seule capacité\n"
	.string "CS que tu utilises dans l'eau.$"

Route20_Text_SwimmermCameronSeen:
	.string "Je suppose qu'il est impossible\n"
	.string "de nager jusqu'à JOHTO.$"

Route20_Text_SwimmermCameronBeaten:
	.string "Aïe !$"

Route20_Text_SwimmermCameronAfterBattle:
	.string "En plus de la mer, je peux aussi\n"
	.string "nager dans les étangs et rivières.$"

Route20_Text_CinnabarIsland_GymUnusedSign:
	.string "Que dit ce panneau ?\p"
	.string "ANCIEN CHAMPION DE\n"
	.string "L'ARENE DE CINNABAR :\l"
	.string "BLAINE$"

