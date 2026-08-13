Route101_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route101_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route101_OnFrame
	.byte 0

Route101_OnTransition:
	call ProfBirch_EventScript_UpdateLocation
	call Route101_EventScript_UpdateTeamRocketVisibility
	end

Route101_EventScript_UpdateTeamRocketVisibility::
	goto_if_set FLAG_TEAM_ROCKET_ROUTE1_DEFEATED, Route101_EventScript_KeepTeamRocketHidden
	goto_if_lt VAR_BIRCH_LAB_STATE, 3, Route101_EventScript_KeepTeamRocketHidden
	clearflag FLAG_HIDE_ROUTE_1_TEAM_ROCKET_ENCOUNTER
	return

Route101_EventScript_KeepTeamRocketHidden::
	setflag FLAG_HIDE_ROUTE_1_TEAM_ROCKET_ENCOUNTER
	return

Route101_OnFrame:
	map_script_2 VAR_ROUTE101_STATE, 0, Route101_EventScript_HideMapNamePopup
	.2byte 0

Route101_EventScript_HideMapNamePopup::
	setflag FLAG_HIDE_MAP_NAME_POPUP
	setvar VAR_ROUTE101_STATE, 1
	end

@ Kanto Saison 1 : la scène de sauvetage du Professeur Chen a été déplacée sur Bourg Palette
@ (LittlerootTown_EventScript_StartBirchRescue) suite à un crash non résolu sur cette carte
@ (cf. CHANGELOG suite 19/20). Les gabarits de mouvement et les textes ci-dessous sont conservés
@ car réutilisés depuis LittlerootTown/scripts.inc.

Route101_EventScript_PreventExitSouth::
	lockall
	msgbox Route101_Text_DontLeaveMe, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, Route101_Movement_PreventExitSouth
	waitmovement 0
	releaseall
	end

Route101_EventScript_PreventExitWest::
	lockall
	msgbox Route101_Text_DontLeaveMe, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, Route101_Movement_PreventExitWest
	waitmovement 0
	releaseall
	end

Route101_EventScript_PreventExitNorth::
	lockall
	msgbox Route101_Text_DontLeaveMe, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, Route101_Movement_PreventExitNorth
	waitmovement 0
	releaseall
	end

Route101_Movement_PreventExitSouth:
	walk_up
	step_end

Route101_Movement_PreventExitWest:
	walk_right
	step_end

Route101_Movement_PreventExitNorth:
	walk_down
	step_end

Route101_Movement_ZigzagoonChaseInCircles:
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_down
	walk_fast_down
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_up
	walk_fast_up
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_down
	walk_fast_down
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_up
	walk_fast_up
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_down
	walk_fast_down
	walk_fast_left
	walk_fast_left
	step_end

Route101_Movement_ZigzagoonChase1:
	walk_fast_up
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_up
	step_end

@ Leftover data? This command is unused.
	step_end

Route101_Movement_ZigzagoonFaceBirch:
	walk_in_place_fast_left
	walk_in_place_fast_left
	walk_in_place_fast_left
	walk_in_place_fast_left
	step_end

Route101_Movement_EnterScene:
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_in_place_faster_left
	step_end

Route101_Movement_BirchRunInCircles:
	walk_fast_up
	walk_fast_up
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_down
	walk_fast_down
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_up
	walk_fast_up
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_down
	walk_fast_down
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_up
	walk_fast_up
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_down
	walk_fast_down
	walk_fast_left
	walk_fast_left
	walk_fast_left
	step_end

Route101_Movement_BirchRunAway1:
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_up
	walk_fast_up
	step_end

@ Leftover data? This command is unused.
	step_end

Route101_Movement_BirchFaceZigzagoon:
	walk_in_place_fast_right
	walk_in_place_fast_right
	walk_in_place_fast_right
	walk_in_place_fast_right
	step_end

Route101_Movement_Unused1:
	walk_up
	walk_up
	step_end

Route101_Movement_Unused2:
	walk_up
	walk_left
	walk_up
	step_end

Route101_EventScript_Youngster::
	msgbox Route101_Text_TakeTiredPokemonToPokeCenter, MSGBOX_NPC
	end

Route101_EventScript_Boy::
	msgbox Route101_Text_WildPokemonInTallGrass, MSGBOX_NPC
	end

Route101_EventScript_RouteSign::
	msgbox Route101_Text_RouteSign, MSGBOX_SIGN
	end

@ Kanto Saison 1 : la remise du PIKACHU (Route101_EventScript_BirchsBag) a été déplacée sur
@ Bourg Palette (LittlerootTown_EventScript_BirchsBag) avec le reste de la scène (cf. ci-dessus).

Route101_EventScript_HideMayInBedroom::
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_BEDROOM
	return

Route101_EventScript_HideBrendanInBedroom::
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_BEDROOM
	return

Route101_Movement_BirchApproachPlayer:
	walk_right
	step_end

Route101_Text_HelpMe:
	.string "À-à l'aide !$"

Route101_Text_PleaseHelp:
	.string "Hé ! Toi, là-bas !\n"
	.string "Je t'en prie, aide-moi !\p"
	.string "Dans mon SAC !\n"
	.string "Il y a une POKé BALL !$"

Route101_Text_DontLeaveMe:
	.string "Qu-qu'est-ce que tu fais ?!\n"
	.string "Ne me laisse pas comme ça !$"

Route101_Text_YouSavedMe:
	.string "PROFESSEUR CHEN : Ouf…\p"
	.string "J'étais dans les hautes herbes en train\n"
	.string "d'étudier un POKéMON sauvage quand\l"
	.string "je me suis fait surprendre.\p"
	.string "Tu m'as sauvé la vie.\n"
	.string "Merci infiniment !\p"
	.string "Oh ?\p"
	.string "Tiens, tu es {PLAYER}{KUN} !\p"
	.string "Ce n'est pas l'endroit pour discuter,\n"
	.string "passe donc à mon LABORATOIRE tout\l"
	.string "à l'heure, d'accord ?$"

Route101_Text_TakeTiredPokemonToPokeCenter:
	.string "Si tes POKéMON sont fatigués, emmène-\n"
	.string "les dans un CENTRE POKéMON.\p"
	.string "Il y en a un tout près, à JADIELLE.$"

Route101_Text_WildPokemonInTallGrass:
	.string "Des POKéMON sauvages peuvent surgir\n"
	.string "des hautes herbes.\p"
	.string "Pour en attraper, il faut s'aventurer\n"
	.string "dans les hautes herbes et chercher.$"

Route101_Text_RouteSign:
	.string "ROUTE 1\n"
	.string "{UP_ARROW} JADIELLE$"


@ ===== Épisode 1.4 — Première rencontre avec la Team Rocket (Arc 1) =====

Route101_EventScript_TeamRocketJessie::
	lock
	faceplayer
	msgbox Route101_Text_RocketJessie_UrgeJames, MSGBOX_DEFAULT
	release
	end

Route101_EventScript_TeamRocketJames::
	trainerbattle_single TRAINER_ROCKET_JAMES_1, Route101_Text_RocketTrio_Devise, Route101_Text_RocketJames_Defeat, Route101_EventScript_TeamRocketDefeated, NO_MUSIC

Route101_EventScript_TeamRocketDefeated::
	msgbox Route101_Text_RocketTrio_Farewell, MSGBOX_DEFAULT
	applymovement LOCALID_ROUTE101_ROCKET_JESSIE, Route101_Movement_RocketFlee
	applymovement LOCALID_ROUTE101_ROCKET_JAMES, Route101_Movement_RocketFlee
	applymovement LOCALID_ROUTE101_ROCKET_MEOWTH, Route101_Movement_RocketFlee
	waitmovement 0
	removeobject LOCALID_ROUTE101_ROCKET_JESSIE
	removeobject LOCALID_ROUTE101_ROCKET_JAMES
	removeobject LOCALID_ROUTE101_ROCKET_MEOWTH
	setflag FLAG_HIDE_ROUTE_1_TEAM_ROCKET_ENCOUNTER
	setflag FLAG_TEAM_ROCKET_ROUTE1_DEFEATED
	release
	end

Route101_EventScript_TeamRocketMeowth::
	lock
	faceplayer
	msgbox Route101_Text_RocketMeowth_Line, MSGBOX_DEFAULT
	release
	end

Route101_Movement_RocketFlee:
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

Route101_Text_RocketJessie_UrgeJames:
	.string "JESSIE : Prépare-toi aux ennuis…\n"
	.string "Allez, James, occupe-toi de ce\l"
	.string "PIKACHU !$"

Route101_Text_RocketTrio_Devise:
	.string "Prépare-toi aux ennuis, et fais que\n"
	.string "ce soit double !\p"
	.string "Ce PIKACHU ferait un excellent\n"
	.string "cadeau pour notre Boss.\p"
	.string "Donne-le-nous bien gentiment… ou\n"
	.string "je m'en charge moi-même !$"

Route101_Text_RocketJames_Defeat:
	.string "Argh ! Ce PIKACHU est bien plus\n"
	.string "fort qu'il n'en a l'air !$"

Route101_Text_RocketTrio_Farewell:
	.string "On dirait qu'on s'envole encore !$"

Route101_Text_RocketMeowth_Line:
	.string "Miaouss ! Ce PIKACHU ne va pas nous\n"
	.string "échapper !$"
