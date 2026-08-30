CeruleanCity_Gym_hns_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CeruleanCity_Gym_OnFrame
	.byte 0

CeruleanCity_Gym_OnFrame::
	map_script_2 VAR_KANTO_ROCKET_STORY_STATE, 2,  CeruleanCity_Gym_EventScript_RocketGrunt
	map_script_2 VAR_TEMP_0, 0,  CeruleanCity_Gym_EventScript_TryMachinePart
	.2byte 0

CeruleanCity_Gym_EventScript_RocketGrunt::
	lock
	applymovement LOCALID_CERULEAN_GYM_ROCKET, CeruleanCity_Gym_Movement_GruntBumpPlayer
	delay 20
	playse SE_BANG
	waitmovement 0
	
	msgbox CeruleanGym_Text_GruntIntro, MSGBOX_DEFAULT
	closemessage
	playse SE_PIN
	applymovement LOCALID_CERULEAN_GYM_ROCKET, Common_Movement_ExclamationMark
	delay 55
	waitmovement LOCALID_CERULEAN_GYM_ROCKET
	applymovement LOCALID_CERULEAN_GYM_ROCKET, CeruleanCity_Gym_Movement_GruntWalkBack
	waitmovement 0
	msgbox CeruleanGym_Text_GruntBigMistake, MSGBOX_DEFAULT
	applymovement LOCALID_CERULEAN_GYM_ROCKET, CeruleanCity_Gym_Movement_GruntWalkToPlayerAgain
	waitmovement 0
	msgbox CeruleanGym_Text_GruntBye, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, CeruleanCity_Gym_Movement_PlayerSlide
	delay 10
	applymovement LOCALID_CERULEAN_GYM_ROCKET, CeruleanCity_Gym_Movement_GruntLeave
	waitmovement 0
	fadescreenswapbuffers FADE_TO_BLACK
	playse SE_EXIT
	setflag FLAG_HIDE_CERULEAN_GYM_ROCKET
	removeobject LOCALID_CERULEAN_GYM_ROCKET
	fadescreenswapbuffers FADE_FROM_BLACK
	clearflag FLAG_HIDE_CERULEAN_CAPE_ROCKET
	setvar VAR_KANTO_ROCKET_STORY_STATE, 3
	release
	end

CeruleanCity_Gym_EventScript_TryMachinePart::
	goto_if_eq VAR_KANTO_ROCKET_STORY_STATE, 5, CeruleanCity_Gym_EventScript_MachinePart
	setvar VAR_TEMP_0, 1
	end

CeruleanCity_Gym_EventScript_MachinePart::
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, CeruleanCity_Movement_CameraPanUp
	waitmovement 0
	delay 30
	setvar VAR_0x8004, 5  @ x
	setvar VAR_0x8005, 13  @ y
	playse SE_PIN
	call Route110_TrickHouseEntrance_EventScript_DoHidingSpotSparkle
	delay 55
	applymovement OBJ_EVENT_ID_CAMERA, CeruleanCity_Movement_CameraPanDown
	waitmovement 0
	special RemoveCameraObject
	setvar VAR_TEMP_0, 1
	end

CeruleanCity_Gym_EventScript_MachinePartItem::
	goto_if_lt VAR_KANTO_ROCKET_STORY_STATE, 5, CeruleanCity_Gym_EventScript_DoNothing
	goto_if_ge VAR_KANTO_ROCKET_STORY_STATE, 6, CeruleanCity_Gym_EventScript_DoNothing
	finditem ITEM_MACHINE_PART
	setvar VAR_KANTO_ROCKET_STORY_STATE, 6
	setflag FLAG_HIDDEN_ITEM_MACHINE_PART
	end

CeruleanCity_Gym_EventScript_DoNothing::
	end

CeruleanCity_Movement_CameraPanUp:
	walk_up
	walk_up
	walk_up
	step_end

CeruleanCity_Movement_CameraPanDown:
	walk_down
	walk_down
	walk_down
	step_end

CeruleanCity_Gym_Movement_GruntBumpPlayer:
	walk_fast_down
	walk_fast_down
	walk_in_place_fast_down
	lock_facing_direction
	jump_up
	unlock_facing_direction
	delay_16
	walk_down
	step_end

CeruleanCity_Gym_Movement_GruntWalkBack:
	lock_facing_direction
	walk_slow_up
	walk_slow_up
	unlock_facing_direction
	step_end

CeruleanCity_Gym_Movement_GruntWalkToPlayerAgain:
	walk_down
	walk_down
	step_end

CeruleanCity_Gym_Movement_PlayerSlide:
	lock_facing_direction
	walk_fast_right
	unlock_facing_direction
	step_end

CeruleanCity_Gym_Movement_GruntLeave:
	walk_fast_down
	walk_in_place_fast_down
	step_end

CeruleanCity_Gym_EventScript_Statue::
	lockall
	goto_if_set FLAG_BADGE10_GET, CeruleanCity_Gym_EventScript_GymStatuePostVictory
	msgbox CeruleanCity_Gym_Text_GymStatue
	releaseall
	end

CeruleanCity_Gym_EventScript_StatueDuringRocket::
	msgbox CeruleanGym_Text_Note, MSGBOX_SIGN
	end 

CeruleanCity_Gym_EventScript_GymStatuePostVictory::
	msgbox CeruleanCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

CeruleanGym_EventScript_Diana::
	trainerbattle_single TRAINER_DIANA_HNS, CeruleanGym_Text_SwimmerfDianaSeen, CeruleanGym_Text_SwimmerfDianaBeaten
	msgbox CeruleanGym_Text_SwimmerfDianaAfter, MSGBOX_AUTOCLOSE
	end

CeruleanGym_EventScript_Briana::
	trainerbattle_single TRAINER_BRIANA_HNS, CeruleanGym_Text_SwimmerfBrianaSeen, CeruleanGym_Text_SwimmerfBrianaBeaten
	msgbox CeruleanGym_Text_SwimmerfBrianaAfter, MSGBOX_AUTOCLOSE
	end

CeruleanGym_EventScript_Parker::
	trainerbattle_single TRAINER_PARKER_HNS, CeruleanGym_Text_SwimmermParkerSeen, CeruleanGym_Text_SwimmermParkerBeaten
	msgbox CeruleanGym_Text_SwimmermParkerAfter, MSGBOX_AUTOCLOSE
	end

CeruleanCity_Gym_EventScript_GymGuy::
	goto_if_set FLAG_DEFEATED_CERULEAN_GYM, CeruleanCity_Gym_EventScript_GymGuyAfter
	msgbox CeruleanGym_Text_GuideIntro, MSGBOX_NPC
	end

CeruleanCity_Gym_EventScript_GymGuyAfter::
	msgbox CeruleanGym_Text_GuideWin, MSGBOX_NPC
	end

CeruleanCity_Gym_Text_GymStatue::
    .string "ARÈNE POKéMON DE CERULEAN\n"
    .string "CHAMPIONNE : ONDINE\p"
	.string "DRESSEURS VICTORIEUX :\n"
	.string "BLUE, RED$"

CeruleanCity_Gym_Text_GymStatuePlayerWon::
    .string "ARÈNE POKéMON DE CERULEAN\n"
    .string "CHAMPIONNE : ONDINE\p"
    .string "DRESSEURS VICTORIEUX :\n"
	.string "BLUE, RED, {PLAYER}$"

CeruleanGym_Text_Note:
	.string "Désolée, je serai absente un\n"
	.string "moment. ONDINE, CHAMPIONNE\p"
	.string "Puisqu'ONDINE est absente, nous\n"
	.string "le sommes aussi.\l"
	.string "LES DRESSEURS DE L'ARÈNE$"


CeruleanCity_Gym_EventScript_Misty::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_CERULEAN_GYM, CeruleanCity_Gym_EventScript_Misty_Defeated
	call HeartSoul_EventScript_OndineDoute
	msgbox CeruleanGym_Text_MistyIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_MISTY_HNS, CeruleanGym_Text_MistyWinLoss
	message CeruleanGym_Text_ReceivedBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	setflag FLAG_BADGE10_GET
	setflag FLAG_DEFEATED_CERULEAN_GYM
	call HeartSoul_EventScript_OndineConvaincue
	addvar VAR_NUM_BADGES, 1
	setvar VAR_0x8008, 10
	call Common_EventScript_SetGymTrainers_hns
	msgbox CeruleanGym_Text_Misty_GiveTM, MSGBOX_DEFAULT
	giveitem ITEM_TM_WATER_PULSE
	msgbox CeruleanGym_Text_Misty_ExplainTM, MSGBOX_DEFAULT
	clearflag FLAG_HIDE_CAPE_MISTY
	call_if_eq VAR_SUICUNE_ENCOUNTERS, 5, CeruleanCity_Gym_SetSuicuneScene
	closemessage
	release
	end

CeruleanCity_Gym_SetSuicuneScene::
	setvar VAR_SUICUNE_ENCOUNTERS, 6
	clearflag FLAG_HIDE_ROUTE25_SUICUNE
	return

CeruleanCity_Gym_EventScript_Misty_Defeated::
	msgbox CeruleanGym_Text_MistyFightDone, MSGBOX_DEFAULT
	closemessage
	release
	end

CeruleanGym_Text_GruntIntro:
	.string "Oups ! Moi désolé ! Toi pas\n"
	.string "blessé, hein ?\p"
	.string "Moi très occupé. Pas le temps de\n"
	.string "parler avec toi. Pas bon pour moi\l"
	.string "si quelqu'un me voit.$"

CeruleanGym_Text_GruntBigMistake:
	.string "Oh non ! Toi déjà m'avoir vu !\n"
	.string "Moi faire grosse erreur !$"

CeruleanGym_Text_GruntBye:
	.string "Hé, toi ! Oublie que tu m'as vu,\n"
	.string "d'accord ?\p"
	.string "Toi voir, entendre, savoir rien,\p"
	.string "d'accord ? Bye, gamin ! Rien !\p"
	.string "Bye-bye a go-go !$"

CeruleanGym_Text_Note1:
	.string "Désolée, je serai absente un\n"
	.string "moment. ONDINE, CHAMPIONNE\p"
	.string "Puisqu'ONDINE est absente, nous\n"
	.string "le sommes aussi. DRESSEURS$"

CeruleanGym_Text_MistyIntro:
	.string "ONDINE : Je t'attendais, petite\n"
	.string "peste !\p"
	.string "Tu as peut-être plein de BADGES\n"
	.string "d'ARÈNE de JOHTO, mais ne me\l"
	.string "sous-estime pas trop.\p"
	.string "Mes POKéMON de type EAU sont\n"
	.string "redoutables !$"

CeruleanGym_Text_MistyWinLoss:
	.string "ONDINE : Tu es vraiment doué…\p"
	.string "Je dois admettre que tu es\n"
	.string "talentueux…\p"
	.string "Tiens, voici le CASCADEBADGE.$"

CeruleanGym_Text_ReceivedBadge:
	.string "{PLAYER} a reçu le CASCADEBADGE.$"

CeruleanGym_Text_MistyFightDone:
	.string "ONDINE : Y a-t-il beaucoup de\n"
	.string "dresseurs forts à JOHTO ? Je veux\l"
	.string "dire, comme toi.\p"
	.string "Je voyagerai un jour, pour\n"
	.string "affronter des dresseurs\l"
	.string "talentueux.$"

CeruleanGym_Text_MistyRematch:
	.string "ONDINE : Waouh, tu es maintenant\n"
	.string "CHAMPION de KANTO !\p"
	.string "C'est incroyable ! Tu as fait du\n"
	.string "chemin, {PLAYER}.\p"
	.string "Mais je ne vais pas reculer.\p"
	.string "Tu veux une revanche contre moi ?$"

CeruleanGym_Text_MistyRematchDefeat:
	.string "ONDINE : Tu es vraiment doué…$"

CeruleanGym_Text_SwimmerfDianaSeen:
	.string "Désolée pour mon absence. On y\n"
	.string "va !$"

CeruleanGym_Text_SwimmerfDianaBeaten:
	.string "J'abandonne ! Tu as gagné !$"

CeruleanGym_Text_SwimmerfDianaAfter:
	.string "Je vais nager tranquillement.$"

CeruleanGym_Text_SwimmerfBrianaSeen:
	.string "Ne te laisse pas déstabiliser\n"
	.string "par ma nage élégante.$"

CeruleanGym_Text_SwimmerfBrianaBeaten:
	.string "Oh, tu m'as calmement battue…$"

CeruleanGym_Text_SwimmerfBrianaAfter:
	.string "Ne sois pas trop fier de m'avoir\n"
	.string "battue.\p"
	.string "ONDINE t'écrasera si tu baisses\n"
	.string "ta garde.$"

CeruleanGym_Text_SwimmermParkerSeen:
	.string "Glouglou…\p"
	.string "Je suis le premier ! Viens me\n"
	.string "chercher !$"

CeruleanGym_Text_SwimmermParkerBeaten:
	.string "C'est pas possible…$"

CeruleanGym_Text_SwimmermParkerAfter:
	.string "ONDINE s'est beaucoup améliorée\n"
	.string "ces dernières années.\p"
	.string "Ne baisse pas ta garde, ou tu\n"
	.string "seras écrasé !$"

CeruleanGym_Text_GuideIntro:
	.string "Yo ! Futur CHAMPION !\p"
	.string "Puisqu'ONDINE était absente, moi\n"
	.string "aussi je suis sorti.\p"
	.string "Pour m'amuser aussi. Héhéhé.$"

CeruleanGym_Text_GuideWin:
	.string "Oh là, tu m'as montré à quel\n"
	.string "point tu es fort.\p"
	.string "Comme toujours, c'était un combat\n"
	.string "du tonnerre !$"

CeruleanGym_Text_Misty_GiveTM:
	.string "Tiens, prends ça aussi.$"

CeruleanGym_Text_Misty_ExplainTM:
	.string "C'est la CT de WATER PULSE.\p"
	.string "Elle ne rate jamais et provoque\n"
	.string "parfois la confusion !$"
