Route101_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route101_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route101_OnFrame
	.byte 0

Route101_OnTransition:
	call ProfBirch_EventScript_UpdateLocation
	call Route101_EventScript_UpdateTeamRocketVisibility
	end

@ Kanto Saison 1 (suite 28) : Jessie et James apparaissaient mal (Jessie invisible, James inaccessible,
@ derrière un buisson décoratif) avec les graphismes d'origine OBJ_EVENT_GFX_ROCKET_F/M — testés en
@ isolation en headless, ces deux graphismes provoquent une corruption mémoire importante sur cette
@ carte dès qu'ils sont affichés, que ce soit par déclaration statique ou par `addobject` (les deux
@ mécanismes testés, aucun des deux n'est fiable ici — contrairement au Professeur Chen en suite 23/25
@ où `addobject` réglait le problème). Remplacés dans Route101/map.json par OBJ_EVENT_GFX_WOMAN_2
@ (Jessie) et OBJ_EVENT_GFX_MAN_3 (James), deux graphismes génériques déjà utilisés sans souci
@ ailleurs dans le jeu, avec des paletteSlot différents de celui du Professeur Chen (PALSLOT_NPC_4)
@ présent sur cette même carte. James repositionné de (9,17) à (10,17) pour le sortir de la case
@ décorative de buisson et l'aligner avec Miaouss (10,18) sur le chemin dégagé. Miaouss (sprite
@ d'espèce, mécanisme différent) n'avait lui aucun problème et reste inchangé. Mécanisme de
@ déclenchement revenu à la version d'origine (`clearflag` déclenché depuis OnTransition, comme
@ PetalburgWoods qui utilisait les graphismes Rocket d'origine avec succès pour la scène Team
@ Aqua/Devon).
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
