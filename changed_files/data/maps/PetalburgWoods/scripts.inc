PetalburgWoods_MapScripts::
	.byte 0

@ Kanto Saison 1 : remplace la scène Team Aqua/Devon Corp (incompatible avec notre histoire) par une
@ réapparition de la Team Rocket, cohérente avec Route 1 (Episode 1.4). Jessie combat cette fois, James
@ l'encourage — inversion des rôles pour faire vivre le duo dans la durée (cf. CHANGELOG Session 4).
@ Les local IDs GRUNT/DEVON_EMPLOYEE sont conservés tels quels (minimise le diff) mais représentent
@ désormais Jessie et James.
@
@ Kanto Saison 1 (suite 31) : graphismes OBJ_EVENT_GFX_ROCKET_F/M remplacés par OBJ_EVENT_GFX_WOMAN_2/
@ OBJ_EVENT_GFX_MAN_3 (retour de test : plantage pendant/après le combat, écran figé sur écran-titre).
@ Même cause que le fix Route101 (suite 28) : ces deux graphismes provoquent une corruption mémoire
@ sur les cartes au format Emerald de ce hack — voir le commentaire de Route101/scripts.inc pour le
@ détail du diagnostic. Jessie/James utilisent donc désormais les mêmes graphismes génériques que sur
@ Route 1, pour une apparence cohérente entre les deux rencontres.
PetalburgWoods_EventScript_DevonResearcherLeft::
	lockall
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, Common_Movement_ExclamationMark
	waitmovement 0
	playbgm MUS_ENCOUNTER_AQUA, FALSE
	msgbox PetalburgWoods_Text_RocketJames_UrgeJessie, MSGBOX_DEFAULT
	closemessage
	trainerbattle_single TRAINER_ROCKET_JESSIE_2, PetalburgWoods_Text_RocketTrio_Devise, PetalburgWoods_Text_RocketJessie_Defeat, PetalburgWoods_EventScript_RocketDefeated, NO_MUSIC
	end

PetalburgWoods_EventScript_DevonResearcherRight::
	lockall
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, Common_Movement_ExclamationMark
	waitmovement 0
	playbgm MUS_ENCOUNTER_AQUA, FALSE
	msgbox PetalburgWoods_Text_RocketJames_UrgeJessie, MSGBOX_DEFAULT
	closemessage
	trainerbattle_single TRAINER_ROCKET_JESSIE_2, PetalburgWoods_Text_RocketTrio_Devise, PetalburgWoods_Text_RocketJessie_Defeat, PetalburgWoods_EventScript_RocketDefeated, NO_MUSIC
	end

PetalburgWoods_EventScript_RocketDefeated::
	msgbox PetalburgWoods_Text_RocketMeowth_Line, MSGBOX_DEFAULT
	closemessage
	msgbox PetalburgWoods_Text_RocketTrio_Farewell, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, PetalburgWoods_Movement_RocketFlee
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, PetalburgWoods_Movement_RocketFlee
	waitmovement 0
	removeobject LOCALID_PETALBURG_WOODS_GRUNT
	removeobject LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE
	setvar VAR_PETALBURG_WOODS_STATE, 1
	releaseall
	end

PetalburgWoods_Movement_RocketFlee:
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

PetalburgWoods_EventScript_Boy1::
	msgbox PetalburgWoods_Text_StayOutOfTallGrass, MSGBOX_NPC
	end

PetalburgWoods_EventScript_Boy2::
	msgbox PetalburgWoods_Text_HiddenItemsExplanation, MSGBOX_NPC
	end

PetalburgWoods_EventScript_Girl::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_MIRACLE_SEED, PetalburgWoods_EventScript_ExplainMiracleSeed
	msgbox PetalburgWoods_Text_TryUsingThisItem, MSGBOX_DEFAULT
	giveitem ITEM_MIRACLE_SEED
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_MIRACLE_SEED
	release
	end

PetalburgWoods_EventScript_ExplainMiracleSeed::
	msgbox PetalburgWoods_Text_MiracleSeedExplanation, MSGBOX_DEFAULT
	release
	end

PetalburgWoods_EventScript_Sign1::
	msgbox PetalburgWoods_Text_TrainerTipsExperience, MSGBOX_SIGN
	end

PetalburgWoods_EventScript_Sign2::
	msgbox PetalburgWoods_Text_TrainerTipsPP, MSGBOX_SIGN
	end

PetalburgWoods_EventScript_Lyle::
	trainerbattle_single TRAINER_LYLE, PetalburgWoods_Text_GoBugPokemonTeam, PetalburgWoods_Text_ICouldntWin
	msgbox PetalburgWoods_Text_ImOutOfPokeBalls, MSGBOX_AUTOCLOSE
	end

PetalburgWoods_EventScript_James::
	trainerbattle_single TRAINER_JAMES_1, PetalburgWoods_Text_InstantlyPopularWithBugPokemon, PetalburgWoods_Text_CantBePopularIfILose, PetalburgWoods_EventScript_TryRegisterJames
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, PetalburgWoods_EventScript_JamesRematch
	setvar VAR_0x8004, TRAINER_JAMES_1
	specialvar VAR_RESULT, IsTrainerRegistered
	goto_if_eq VAR_RESULT, FALSE, PetalburgWoods_EventScript_TryRegisterJames2
	msgbox PetalburgWoods_Text_PeopleRespectYou, MSGBOX_DEFAULT
	release
	end

PetalburgWoods_EventScript_TryRegisterJames::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	goto_if_set FLAG_HAS_MATCH_CALL, PetalburgWoods_EventScript_RegisterJames
	release
	end

PetalburgWoods_EventScript_RegisterJames::
	msgbox PetalburgWoods_Text_IWantRematch1, MSGBOX_DEFAULT
	register_matchcall TRAINER_JAMES_1
	release
	end

PetalburgWoods_EventScript_TryRegisterJames2::
	goto_if_set FLAG_HAS_MATCH_CALL, PetalburgWoods_EventScript_RegisterJames2
	msgbox PetalburgWoods_Text_PeopleRespectYou, MSGBOX_DEFAULT
	release
	end

PetalburgWoods_EventScript_RegisterJames2::
	msgbox PetalburgWoods_Text_IWantRematch2, MSGBOX_DEFAULT
	register_matchcall TRAINER_JAMES_1
	release
	end

PetalburgWoods_EventScript_JamesRematch::
	trainerbattle_rematch TRAINER_JAMES_1, PetalburgWoods_Text_MyPokemonHaveGrown, PetalburgWoods_Text_CantBePopularIfLose
	msgbox PetalburgWoods_Text_IveBeenTrainingHard, MSGBOX_AUTOCLOSE
	end

PetalburgWoods_Text_RocketJames_UrgeJessie:
	.string "JAMES : Tiens, tiens…\n"
	.string "On dirait qu'on a de la visite,\l"
	.string "Jessie.\p"
	.string "Occupe-toi donc de ce dresseur\n"
	.string "pendant que je termine le filet !$"

PetalburgWoods_Text_RocketTrio_Devise:
	.string "Prépare-toi aux ennuis, et fais que\n"
	.string "ce soit double !\p"
	.string "Cette forêt grouille de POKéMON,\n"
	.string "et notre Boss les adorerait tous.\p"
	.string "Laisse-nous travailler tranquillement…\n"
	.string "ou affronte-moi !$"

PetalburgWoods_Text_RocketJessie_Defeat:
	.string "Quoi ? Encore un dresseur costaud !\n"
	.string "C'est d'un agaçant !$"

PetalburgWoods_Text_RocketTrio_Farewell:
	.string "Bon, cette forêt ne valait pas le\n"
	.string "coup de toute façon !$"

PetalburgWoods_Text_RocketMeowth_Line:
	.string "Miaouss ! On ferait mieux de filer\n"
	.string "avant que ça tourne mal !$"

PetalburgWoods_Text_GoBugPokemonTeam:
	.string "I caught a whole bunch of POKéMON!\p"
	.string "Go, go, go!\n"
	.string "My BUG POKéMON team!$"

PetalburgWoods_Text_ICouldntWin:
	.string "I have all these POKéMON,\n"
	.string "but I couldn't win…$"

PetalburgWoods_Text_ImOutOfPokeBalls:
	.string "I caught a bunch of POKéMON.\n"
	.string "Now I'm out of POKé BALLS.$"

PetalburgWoods_Text_InstantlyPopularWithBugPokemon:
	.string "If you take BUG POKéMON to school,\n"
	.string "you get to be instantly popular!$"

PetalburgWoods_Text_CantBePopularIfILose:
	.string "I can't be popular if I lose.$"

PetalburgWoods_Text_PeopleRespectYou:
	.string "If you have a big BUG POKéMON,\n"
	.string "people respect you for it.$"

PetalburgWoods_Text_IWantRematch1:
	.string "I want a rematch when my BUG\n"
	.string "POKéMON grow up!\p"
	.string "I'm registering you in my POKéNAV!$"

PetalburgWoods_Text_IWantRematch2:
	.string "I want a rematch when my BUG \n"
	.string "POKéMON grow up!\p"
	.string "I'm registering you in my POKéNAV!$"

PetalburgWoods_Text_MyPokemonHaveGrown:
	.string "My BUG POKéMON have grown.\n"
	.string "I'll be popular again.$"

PetalburgWoods_Text_CantBePopularIfLose:
	.string "I can't be popular if I lose.$"

PetalburgWoods_Text_IveBeenTrainingHard:
	.string "You get to be popular if you have\n"
	.string "strong POKéMON, right?\l"
	.string "So, I've been training hard.$"

PetalburgWoods_Text_StayOutOfTallGrass:
	.string "Yo, there!\n"
	.string "Your POKéMON doing okay?\p"
	.string "If your POKéMON are weak and you want\n"
	.string "to avoid battles, you should stay out\l"
	.string "of tall grass.$"

PetalburgWoods_Text_HiddenItemsExplanation:
	.string "Sometimes, there are things on the\n"
	.string "ground even if you can't see them.\p"
	.string "That's why I always check where I'm\n"
	.string "walking.$"

PetalburgWoods_Text_TryUsingThisItem:
	.string "Oh, neat!\n"
	.string "That's the BADGE from RUSTBORO GYM!\p"
	.string "You must be a TRAINER.\n"
	.string "You should try using this item.$"

PetalburgWoods_Text_MiracleSeedExplanation:
	.string "It's a MIRACLE SEED.\n"
	.string "If a POKéMON holds that item, its\l"
	.string "GRASS-type moves become stronger.\p"
	.string "Besides MIRACLE SEED, I think there are\n"
	.string "other convenient items for POKéMON\l"
	.string "to hold.$"

PetalburgWoods_Text_TrainerTipsExperience:
	.string "TRAINER TIPS\p"
	.string "Any POKéMON that appears even once\n"
	.string "in a battle is awarded EXP Points.\p"
	.string "To raise a weak POKéMON, put it at the\n"
	.string "left of the team list.\p"
	.string "As soon as a battle starts, switch it\n"
	.string "out. It will earn EXP Points without\l"
	.string "being exposed to any harm.$"

PetalburgWoods_Text_TrainerTipsPP:
	.string "TRAINER TIPS\p"
	.string "In addition to Hit Points (HP), POKéMON\n"
	.string "have Power Points (PP) that are used to\l"
	.string "make moves during battle.\p"
	.string "If a POKéMON runs out of PP, it must be\n"
	.string "taken to a POKéMON CENTER.$"

