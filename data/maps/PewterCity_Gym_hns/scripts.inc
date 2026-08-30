PewterCity_Gym_hns_MapScripts::
	.byte 0



PewterCity_Gym_EventScript_Jerry::
	trainerbattle_single TRAINER_JERRY_HNS, PewterCity_Gym_Text_CamperJerry_Seen, PewterCity_Gym_Text_CamperJerry_Beaten
	msgbox PewterCity_Gym_Text_CamperJerry_AfterBattle, MSGBOX_AUTOCLOSE
	end

PewterCity_Gym_EventScript_Brock::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_PEWTER_GYM, PewterCity_Gym_EventScript_Brock_Defeated
	call HeartSoul_EventScript_PierreDoute
	msgbox PewterCity_Gym_Text_Brock_Intro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_BROCK_HNS, PewterCity_Gym_Text_Brock_WinLoss
	message PewterCity_Gym_Text_ReceivedBoulderBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox PewterCity_Gym_Text_Brock_GiveTM, MSGBOX_DEFAULT
	giveitem ITEM_TM_ROCK_SLIDE
	msgbox PewterCity_Gym_Text_Brock_ExplainTM, MSGBOX_DEFAULT
	setflag FLAG_BADGE09_GET
	setflag FLAG_DEFEATED_PEWTER_GYM
	call HeartSoul_EventScript_PierreConvaincu
	addvar VAR_NUM_BADGES, 1
	setvar VAR_0x8008, 9
	call Common_EventScript_SetGymTrainers_hns
	clearflag FLAG_HIDE_DIGLETT_BROCK
	closemessage
	release
	end

PewterCity_Gym_EventScript_Brock_Defeated::
	goto_if_unset FLAG_HIDE_DOJO_BROCK, PewterCity_Gym_EventScript_Brock_Trade
	msgbox PewterCity_Gym_Text_Brock_FightDone, MSGBOX_DEFAULT
	closemessage
	release
	end


PewterCity_Gym_Text_Brock_Intro:
	.string "PIERRE : Wow, ce n'est pas si\n"
	.string "fréquent d'avoir un challenger\l"
	.string "venu de JOHTO.\p"
	.string "Je suis PIERRE, le CHEF de\n"
	.string "PEWTER GYM.\p"
	.string "Je suis un expert des POKéMON\n"
	.string "de type ROCHE.\p"
	.string "Mes POKéMON résistent à la\n"
	.string "plupart des attaques\l"
	.string "physiques. Tu auras du mal à\l"
	.string "leur infliger des dégâts.\p"
	.string "Allons-y !$"

PewterCity_Gym_Text_Brock_WinLoss:
	.string "PIERRE : Les puissantes attaques\n"
	.string "de tes POKéMON ont eu raison\l"
	.string "de ma défense de pierre…\p"
	.string "Tu es plus fort que je ne le\n"
	.string "pensais…\p"
	.string "Vas-y, prends ce BADGE.$"

PewterCity_Gym_Text_ReceivedBoulderBadge:
	.string "{PLAYER} a reçu le BOULDERBADGE.$"

PewterCity_Gym_Text_Brock_GiveTM:
	.string "Tiens, prends cette CT.$"

PewterCity_Gym_Text_Brock_ExplainTM:
	.string "Elle contient ROCK SLIDE.\p"
	.string "Elle fait tomber des rochers\n"
	.string "sur l'adversaire, avec une\l"
	.string "chance de provoquer la peur.\p"
	.string "Utilise-la pour l'arrêter net.$"

PewterCity_Gym_Text_Brock_BoulderBadge:
	.string "PIERRE : {PLAYER}, merci. J'ai\n"
	.string "aimé ce combat, même si je\l"
	.string "suis un peu déçu.\p"
	.string "Ce BOULDERBADGE rendra tes\n"
	.string "POKéMON encore plus puissants.$"

PewterCity_Gym_Text_Brock_FightDone:
	.string "PIERRE : Le monde est immense.\n"
	.string "Il y a encore beaucoup de\l"
	.string "dresseurs forts comme toi.\p"
	.string "Attends un peu. Je vais moi\n"
	.string "aussi devenir bien plus fort.$"

PewterCity_Gym_Text_CamperJerry_Seen:
	.string "Les dresseurs de cette arène\n"
	.string "utilisent des POKéMON de type\l"
	.string "ROCHE.\p"
	.string "Le type ROCHE a une DÉFENSE\n"
	.string "élevée.\p"
	.string "Les combats peuvent durer\n"
	.string "longtemps. Tu es prêt ?$"

PewterCity_Gym_Text_CamperJerry_Beaten:
	.string "Je dois gagner ces combats…$"

PewterCity_Gym_Text_CamperJerry_AfterBattle:
	.string "Hé, toi ! Dresseur de JOHTO !\n"
	.string "PIERRE est coriace. Il te fera\l"
	.string "payer si tu le sous-estimes.$"

PewterCity_Gym_Text_Guide:
	.string "Yo ! Futur CHAMPION ! Tu\n"
	.string "déchires tout.\p"
	.string "Tu affrontes les CHEFS d'ARÈNE\n"
	.string "de KANTO ?\p"
	.string "Ce sont des gens forts et\n"
	.string "dévoués, tout comme les CHEFS\l"
	.string "d'ARÈNE de JOHTO.$"

PewterCity_Gym_Text_Guide_Win:
	.string "Yo ! Futur CHAMPION ! Cette\n"
	.string "arène ne t'a pas donné trop\l"
	.string "de fil à retordre.\p"
	.string "Ta façon de mener ce combat\n"
	.string "était vraiment inspirante. Je\l"
	.string "le pense sincèrement.$"

PewterCity_Gym_EventScript_GymGuy::
	goto_if_set FLAG_DEFEATED_PEWTER_GYM, PewterCity_Gym_EventScript_GymGuyAfter
	msgbox PewterCity_Gym_Text_Guide, MSGBOX_NPC
	end

PewterCity_Gym_EventScript_GymGuyAfter::
	msgbox PewterCity_Gym_Text_Guide_Win, MSGBOX_NPC
	end


PewterCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE09_GET, PewterCity_Gym_EventScript_GymStatuePostVictory
	msgbox PewterCity_Gym_Text_GymStatue
	releaseall
	end

PewterCity_Gym_EventScript_GymStatuePostVictory::
	msgbox PewterCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

PewterCity_Gym_Text_GymStatue::
    .string "PEWTER POKéMON GYM\n"
    .string "CHEF : PIERRE\p"
	.string "DRESSEURS VICTORIEUX :\n"
	.string "BLUE, RED$"

PewterCity_Gym_Text_GymStatuePlayerWon::
    .string "PEWTER POKéMON GYM\n"
    .string "CHEF : PIERRE\p"
    .string "DRESSEURS VICTORIEUX :\n"
	.string "BLUE, RED, {PLAYER}$"

PewterCity_Gym_EventScript_Brock_Trade::
	goto_if_set FLAG_PEWTER_GYM_NPC_TRADE_COMPLETED, PewterCity_Gym_EventScript_BrockTradeCompleted
	ingame_trade INGAME_TRADE_BONSLY, PewterCity_Gym_Text_BrockWantToTrade, PewterCity_Gym_EventScript_BrockDeclineTrade, PewterCity_Gym_EventScript_BrockNotRequestedMon, PewterCity_Gym_Text_BrockTakeCare
	setflag FLAG_PEWTER_GYM_NPC_TRADE_COMPLETED
	release
	end

PewterCity_Gym_EventScript_BrockDeclineTrade::
	msgbox PewterCity_Gym_Text_BrockDeclineTrade, MSGBOX_DEFAULT
	release
	end

PewterCity_Gym_EventScript_BrockNotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox PewterCity_Gym_Text_BrockNotRequestedMon, MSGBOX_DEFAULT
	release
	end

PewterCity_Gym_EventScript_BrockTradeCompleted::
	msgbox PewterCity_Gym_Text_BrockTradeCompleted, MSGBOX_DEFAULT
	release
	end

PewterCity_Gym_Text_BrockWantToTrade:
	.string "PIERRE : Hé, {PLAYER}. J'ai\n"
	.string "trouvé ce petit BONSLY en\l"
	.string "m'entraînant dans les\l"
	.string "montagnes.\p"
	.string "Il est mignon, mais je pense\n"
	.string "qu'il lui faut un autre genre\l"
	.string "de dresseur.\p"
	.string "Voudrais-tu échanger un\n"
	.string "{STR_VAR_1} contre mon\l"
	.string "{STR_VAR_2} ?$"

PewterCity_Gym_Text_BrockTakeCare:
	.string "PIERRE : Merci ! Prends bien\n"
	.string "soin de WOODY pour moi !$"

PewterCity_Gym_Text_BrockDeclineTrade:
	.string "PIERRE : Non ? Pas de souci.\n"
	.string "WOODY et moi allons continuer\l"
	.string "à nous entraîner.$"

PewterCity_Gym_Text_BrockNotRequestedMon:
	.string "PIERRE : Ce n'est pas un\n"
	.string "{STR_VAR_1}. Ce n'est pas ce\l"
	.string "que je cherche.$"

PewterCity_Gym_Text_BrockTradeCompleted:
	.string "PIERRE : Comment va WOODY ?\n"
	.string "Les BONSLY sont plus résistants\l"
	.string "qu'ils n'en ont l'air !$"
