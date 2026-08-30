Route2_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route2_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	.byte 0

Route2_OnTransition: 
	callnative SetTimeBasedEncounters
	end

Route2_EventScript_Rob::
	trainerbattle_single TRAINER_ROB_HNS, Route2_Text_BugCatcherRob_Seen, Route2_Text_BugCatcherRob_Beaten, Route2_EventScript_RegisterRob
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route2_EventScript_RematchRob
	msgbox Route2_Text_BugCatcherRob_AfterBattle, MSGBOX_AUTOCLOSE
	end

Route2_EventScript_RegisterRob::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox Route2_Text_RobRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_ROB_HNS
	release
	end

Route2_EventScript_RematchRob::
	trainerbattle_rematch TRAINER_ROB_HNS, Route2_Text_RobRematchIntro, Route2_Text_RobRematchDefeat
	msgbox Route2_Text_RobPostRematch, MSGBOX_AUTOCLOSE
	goto_if_defeated TRAINER_ROB_2_HNS, Route2_EventScript_RematchRobTryGiveItem
	end

Route2_EventScript_RematchRobTryGiveItem::
	checkitem ITEM_TM_X_SCISSOR
	goto_if_eq VAR_RESULT, TRUE, Common_EventScript_NoOp
	giveitem ITEM_TM_X_SCISSOR
	closemessage
	end

Route2_EventScript_Ed::
	trainerbattle_single TRAINER_ED_HNS, Route2_Text_BugCatcherEd_Seen, Route2_Text_BugCatcherEd_Beaten
	msgbox Route2_Text_BugCatcherEd_AfterBattle, MSGBOX_AUTOCLOSE
	end

Route2_EventScript_Doug::
	trainerbattle_single TRAINER_DOUG_HNS, Route2_Text_BugCatcherDoug_Seen, Route2_Text_BugCatcherDoug_Beaten, Route2_EventScript_DougPostBattle
	msgbox Route2_Text_BugCatcherDoug_AfterBattle, MSGBOX_AUTOCLOSE
	end

// Heart & Soul : meme correctif que Route1_EventScript_QuinnPostBattle (voir son commentaire,
// data/maps/Route1_hns/scripts.inc) - event_script requis pour s'executer des la premiere
// victoire, pas seulement au rappel.
Route2_EventScript_DougPostBattle::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox Route2_Text_BugCatcherDoug_AfterBattle, MSGBOX_DEFAULT
	call HeartSoul_EventScript_BucheronsForet
	closemessage
	release
	end

Route2_EventScript_Sign::
	msgbox Route2_Text_Sign, MSGBOX_SIGN
	end

Route2_EventScript_CaveSign::
	msgbox Route2_Text_DiglettsCaveSign, MSGBOX_SIGN
	end

Route2_EventScript_Carbos::
    finditem ITEM_CARBOS
    end

Route2_EventScript_Nugget::
    finditem ITEM_NUGGET
    end

Route2_Text_RobRegister:
	.string "Laisse-moi prendre ton numéro !$"

Route2_Text_RobRematchIntro:
	.string "J'ai attrapé encore plus\n"
	.string "d'insectes ! Regarde ça !$"

Route2_Text_RobRematchDefeat:
	.string "Il me faut de meilleurs insectes !$"

Route2_Text_RobPostRematch:
	.string "J'ai attrapé tous les insectes\n"
	.string "de VIRIDIAN. Y a-t-il une\l"
	.string "meilleure FORÊT quelque part ?$"

Route2_Text_BugCatcherRob_Seen:
	.string "Mes POKéMON insectes sont\n"
	.string "coriaces. Prépare-toi à perdre !$"

Route2_Text_BugCatcherRob_Beaten:
	.string "Je me suis fait rétamer…$"

Route2_Text_BugCatcherRob_AfterBattle:
	.string "Je vais chercher des POKéMON\n"
	.string "insectes plus forts.$"

Route2_Text_BugCatcherEd_Seen:
	.string "Si tu marches en short dans\n"
	.string "les hautes herbes, tu te fais\l"
	.string "griffer, non ?$"

Route2_Text_BugCatcherEd_Beaten:
	.string "Aïe, aïe, aïe !$"

Route2_Text_BugCatcherEd_AfterBattle:
	.string "Ça pique vraiment quand tu\n"
	.string "prends un bain.$"

Route2_Text_BugCatcherDoug_Seen:
	.string "Pourquoi les filles n'aiment\n"
	.string "pas les POKéMON insectes ?$"

Route2_Text_BugCatcherDoug_Beaten:
	.string "Pas bon !$"

Route2_Text_BugCatcherDoug_AfterBattle:
	.string "Les POKéMON insectes sont tout\n"
	.string "mous, comme des peluches, quand\l"
	.string "on leur presse le ventre.\p"
	.string "J'adore cette sensation !$"

Route2_Text_Sign:
	.string "ROUTE 2\p"
	.string "VIRIDIAN CITY - PEWTER CITY$"

Route2_Text_DiglettsCaveSign:
	.string "DIGLETT'S CAVE$"

