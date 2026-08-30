ViridianCity_Gym_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ViridianCity_Gym_OnTransition
	.byte 0

@ Heart & Soul : Blue est le chef de la Team Rocket (voir Cinnabar), pas un Champion
@ d'Arene amical. Meme motif que CinnabarIsland_OnTransition (data/maps/CinnabarIsland_hns/
@ scripts.inc) : setflag simple, pas de msgbox, donc ON_TRANSITION reste sur ici. Le garde sur
@ FLAG_ACTE_5_DEBLOQUE laisse la possibilite de le faire reapparaitre plus tard sans retoucher
@ ce script (meme motif que HeartSoul_EventScript_CinnabarVerrouilleeCheck).
ViridianCity_Gym_OnTransition:
	goto_if_set FLAG_ACTE_5_DEBLOQUE, ViridianCity_Gym_OnTransition_End
	setflag FLAG_HIDE_VIRIDIAN_BLUE
ViridianCity_Gym_OnTransition_End:
	end

ViridianCity_Gym_EventScript_Blue::
	lock
	faceplayer
	goto_if_unset FLAG_HIDE_DOJO_BLUE, ViridianCity_Gym_EventScript_BlueAtDojo
	goto_if_set FLAG_DEFEATED_VIRIDIAN_GYM, ViridianCity_Gym_EventScript_BlueGotoDojo
	goto_if_set FLAG_DEFEATED_VIRIDIAN_GYM, ViridianCity_Gym_EventScript_Blue_Defeated
	msgbox ViridianCity_Gym_Text_LeaderBlue_Before, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_BLUE_HNS, ViridianCity_Gym_Text_LeaderBlue_Win
	message ViridianCity_Gym_Text_ReceivedEarthBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox ViridianCity_Gym_Text_LeaderBlue_After, MSGBOX_DEFAULT
	msgbox ViridianCity_Gym_Text_LeaderBlue_GiveTM, MSGBOX_DEFAULT
	giveitem ITEM_TM_TRICK_ROOM
	msgbox ViridianCity_Gym_Text_LeaderBlue_ExplainTM, MSGBOX_DEFAULT
	setflag FLAG_BADGE16_GET
	setflag FLAG_DEFEATED_VIRIDIAN_GYM
	addvar VAR_NUM_BADGES, 1
	setvar VAR_0x8008, 16
	call Common_EventScript_SetGymTrainers_hns
	closemessage
	setflag FLAG_HIDE_LEAGUE_BLACKBELT
	setflag FLAG_HIDE_SEAFOAM_BLAINE
	clearflag FLAG_HIDE_CINNABAR_BLAINE
	setvar VAR_OAK_MT_SILVER_CALL, 1
	release
	end

ViridianCity_Gym_EventScript_Blue_Defeated::
	msgbox ViridianCity_Gym_Text_LeaderBlue_Epilogue, MSGBOX_DEFAULT
	closemessage
	release
	end

ViridianCity_Gym_EventScript_BlueGotoDojo::
	msgbox ViridianCity_Gym_Text_BlueGotoDojo, MSGBOX_DEFAULT
	closemessage
	clearflag FLAG_HIDE_DOJO_BLUE
	release
	end

ViridianCity_Gym_Text_BlueGotoDojo:
	.string "BLUE: Déjà de retour pour plus ?\p"
	.string "Si tu veux retenter ta chance\n"
	.string "contre moi, viens au FIGHTING\l"
	.string "DOJO !$"

ViridianCity_Gym_EventScript_BlueAtDojo::
	msgbox ViridianCity_Gym_Text_LeaderBlue_Epilogue, MSGBOX_DEFAULT
	closemessage
	release
	end

ViridianCity_Gym_Text_LeaderBlue_Before:
	.string "BLUE: Yo ! T'es enfin arrivé,\n"
	.string "hein ?\p"
	.string "J'étais pas d'humeur à CINNABAR,\p"
	.string "mais là, je suis prêt à te\n"
	.string "combattre.\p"
	.string "…\p"
	.string "Tu me dis que t'as conquis toutes\n"
	.string "les ARENES de JOHTO ?\p"
	.string "Héhé ! Elles doivent être\n"
	.string "sacrément minables, alors.\p"
	.string "Hé, t'inquiète pas pour ça.\p"
	.string "Je saurai si t'es bon ou pas en\n"
	.string "te combattant maintenant.\p"
	.string "Prêt, CHAMPION DE JOHTO ?$"

ViridianCity_Gym_Text_LeaderBlue_Win:
	.string "BLUE: Quoi ?\p"
	.string "Comment j'ai pu perdre contre\n"
	.string "toi ?\p"
	.string "…\p"
	.string "Tch, bon d'accord… Tiens, prends\n"
	.string "ça, c'est l'EARTHBADGE.$"

ViridianCity_Gym_Text_ReceivedEarthBadge:
	.string "{PLAYER} a reçu l'EARTHBADGE.$"

ViridianCity_Gym_Text_LeaderBlue_After:
	.string "BLUE: …\p"
	.string "D'accord, j'avais tort. T'es du\n"
	.string "genre vrai. T'es un bon dresseur.\p"
	.string "Mais je vais te battre un jour.\p"
	.string "Oublie pas ça !$"

ViridianCity_Gym_Text_LeaderBlue_GiveTM:
	.string "Prends ça avec toi.$"

ViridianCity_Gym_Text_LeaderBlue_ExplainTM:
	.string "Voici TRICK ROOM.\p"
	.string "Tous les POKéMON en combat changent\n"
	.string "de vitesse. Les lents passent en\l"
	.string "premier, les rapides en dernier.\p"
	.string "Utilise-la pour renverser un\n"
	.string "combat.$"

ViridianCity_Gym_Text_LeaderBlue_Epilogue:
	.string "BLUE: Écoute-moi bien.\p"
	.string "T'as intérêt à ne pas perdre\n"
	.string "avant que je te batte. Compris ?$"


ViridianCity_Gym_EventScript_GymGuy::
	goto_if_set FLAG_DEFEATED_VIRIDIAN_GYM, ViridianCity_Gym_EventScript_GymGuyAfter
	msgbox ViridianCity_Gym_Text_Guide, MSGBOX_NPC
	end

ViridianCity_Gym_EventScript_GymGuyAfter::
	msgbox ViridianCity_Gym_Text_Guide_Win, MSGBOX_NPC
	end

ViridianCity_Gym_Text_Guide:
	.string "…\p"
	.string "Personne n'a revu le Champion\n"
	.string "depuis l'attaque de CINNABAR.\p"
	.string "L'Arene est fermee jusqu'a nouvel\n"
	.string "ordre.$"

ViridianCity_Gym_Text_Guide_Win:
	.string "Waouh, t'es vraiment fort…\p"
	.string "C'était un combat sacrément\n"
	.string "inspirant. Ça m'a arraché des\l"
	.string "larmes.$"


ViridianCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE16_GET, ViridianCity_Gym_EventScript_GymStatuePostVictory
	msgbox ViridianCity_Gym_Text_GymStatue
	releaseall
	end

ViridianCity_Gym_EventScript_GymStatuePostVictory::
	msgbox ViridianCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

ViridianCity_Gym_Text_GymStatue::
    .string "ARENE POKéMON DE VIRIDIAN\n"
    .string "CHAMPION : BLUE\p"
	.string "DRESSEURS VAINQUEURS :\n"
	.string "BLUE, RED$"

ViridianCity_Gym_Text_GymStatuePlayerWon::
    .string "ARENE POKéMON DE VIRIDIAN\n"
    .string "CHAMPION : BLUE\p"
    .string "DRESSEURS VAINQUEURS :\n"
	.string "BLUE, RED, {PLAYER}$"

