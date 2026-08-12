RustboroCity_Gym_MapScripts::
	.byte 0

RustboroCity_Gym_EventScript_Roxanne::
	trainerbattle_single TRAINER_ROXANNE_1, RustboroCity_Gym_Text_RoxanneIntro, RustboroCity_Gym_Text_RoxanneDefeat, RustboroCity_Gym_EventScript_RoxanneDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, RustboroCity_Gym_EventScript_RoxanneRematch
	goto_if_unset FLAG_RECEIVED_TM_ROCK_TOMB, RustboroCity_Gym_EventScript_GiveRockTomb
	msgbox RustboroCity_Gym_Text_RoxannePostBattle, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_RoxanneDefeated::
	message RustboroCity_Gym_Text_ReceivedStoneBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox RustboroCity_Gym_Text_StoneBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_RUSTBORO_GYM
	setflag FLAG_BADGE01_GET
	setvar VAR_RUSTBORO_CITY_STATE, 1
	setvar VAR_0x8008, 1
	call Common_EventScript_SetGymTrainers
	goto RustboroCity_Gym_EventScript_GiveRockTomb
	end

RustboroCity_Gym_EventScript_GiveRockTomb::
	giveitem ITEM_TM_ROCK_TOMB
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM_ROCK_TOMB
	msgbox RustboroCity_Gym_Text_ExplainRockTomb, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_RoxanneRematch::
	trainerbattle_rematch_double TRAINER_ROXANNE_1, RustboroCity_Gym_Text_RoxannePreRematch, RustboroCity_Gym_Text_RoxanneRematchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox RustboroCity_Gym_Text_RoxannePostRematch, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_Josh::
	trainerbattle_single TRAINER_JOSH, RustboroCity_Gym_Text_JoshIntro, RustboroCity_Gym_Text_JoshDefeat
	msgbox RustboroCity_Gym_Text_JoshPostBattle, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_Tommy::
	trainerbattle_single TRAINER_TOMMY, RustboroCity_Gym_Text_TommyIntro, RustboroCity_Gym_Text_TommyDefeat
	msgbox RustboroCity_Gym_Text_TommyPostBattle, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_Marc::
	trainerbattle_single TRAINER_MARC, RustboroCity_Gym_Text_MarcIntro, RustboroCity_Gym_Text_MarcDefeat
	msgbox RustboroCity_Gym_Text_MarcPostBattle, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_RUSTBORO_GYM, RustboroCity_Gym_EventScript_GymGuidePostVictory
	msgbox RustboroCity_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_GymGuidePostVictory::
	msgbox RustboroCity_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE01_GET, RustboroCity_Gym_EventScript_GymStatueCertified
	goto RustboroCity_Gym_EventScript_GymStatue
	end

RustboroCity_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE01_GET, RustboroCity_Gym_EventScript_GymStatueCertified
	goto RustboroCity_Gym_EventScript_GymStatue
	end

RustboroCity_Gym_EventScript_GymStatueCertified::
	msgbox RustboroCity_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

RustboroCity_Gym_EventScript_GymStatue::
	msgbox RustboroCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

RustboroCity_Gym_EventScript_RegisterRoxanne::
	lockall
	pokenavcall RustboroCity_Gym_Text_RoxanneRegisterCall
	waitmessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox RustboroCity_Gym_Text_RegisteredRoxanne, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_ROXANNE_MATCH_CALL
	clearflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	releaseall
	end

RustboroCity_Gym_Text_GymGuideAdvice:
	.string "Hé, comment ça va ?\p"
	.string "Écoute-moi bien, l'ami !\n"
	.string "Tu veux devenir CHAMPION ?\p"
	.string "Moi, dresseur, je ne le suis pas,\n"
	.string "mais je peux te filer de bons\l"
	.string "conseils.\p"
	.string "C'est décidé, alors ! On vise le\n"
	.string "CHAMPIONNAT POKéMON ensemble !\p"
	.string "Ton boulot de dresseur, c'est de\n"
	.string "récolter les BADGES d'arène, non ?\p"
	.string "Mais les champions d'arène ne\n"
	.string "sont pas des tendres !\p"
	.string "Et c'est là que j'interviens !\n"
	.string "Je suis là pour te filer des tuyaux !\p"
	.string "ONDINE, la championne d'arène,\n"
	.string "utilise des POKéMON de type EAU.\p"
	.string "Le type EAU encaisse bien, mais\n"
	.string "il redoute les attaques ÉLECTRIK\l"
	.string "et PLANTE.\p"
	.string "Repasse me voir si tu bats la\n"
	.string "championne d'arène.\p"
	.string "Allez, fonce !$"

RustboroCity_Gym_Text_GymGuidePostVictory:
	.string "Waouh ! Quelle victoire !\n"
	.string "Mes encouragements ont dû payer !\p"
	.string "Génial !\n"
	.string "Regarde ta CARTE DRESSEUR.\p"
	.string "Le BADGE CASCADE que tu as gagné\n"
	.string "doit y être enregistré.\p"
	.string "Autrement dit…\p"
	.string "Tu viens de gravir la première\n"
	.string "marche vers le CHAMPIONNAT !\p"
	.string "Ça doit faire un sacré effet !$"

RustboroCity_Gym_Text_JoshIntro:
	.string "Ne sous-estime pas les dresseurs\n"
	.string "de cette arène !\p"
	.string "Je vais te montrer ce qu'on a\n"
	.string "dans le ventre !$"

RustboroCity_Gym_Text_JoshDefeat:
	.string "Tu étais trop fort pour moi…$"

RustboroCity_Gym_Text_JoshPostBattle:
	.string "Tu n'as encore rien vu de la\n"
	.string "puissance des POKéMON EAU !$"

RustboroCity_Gym_Text_TommyIntro:
	.string "Si tu n'arrives pas à me battre,\n"
	.string "tu n'as aucune chance contre\l"
	.string "ONDINE !$"

RustboroCity_Gym_Text_TommyDefeat:
	.string "Waouh ! Tu as du potentiel !$"

RustboroCity_Gym_Text_TommyPostBattle:
	.string "ONDINE est bien plus forte que\n"
	.string "moi.\p"
	.string "Tiens-toi prêt !$"

RustboroCity_Gym_Text_MarcIntro:
	.string "On est peut-être en pleine ville,\n"
	.string "mais qu'importe.\p"
	.string "Mes POKéMON EAU ont besoin\n"
	.string "d'espace pour s'exprimer !$"

RustboroCity_Gym_Text_MarcDefeat:
	.string "Oh là là !\n"
	.string "Quel adversaire coriace !$"

RustboroCity_Gym_Text_MarcPostBattle:
	.string "Je dois reconnaître un truc à\n"
	.string "notre championne.\p"
	.string "Elle a eu du flair de choisir le\n"
	.string "type EAU si jeune.$"

RustboroCity_Gym_Text_RoxanneIntro:
	.string "ONDINE : On se retrouve, {PLAYER}.\p"
	.string "Je t'avais bien dit qu'on n'en\n"
	.string "resterait pas là, sur la Route 2.\p"
	.string "Ici, c'est chez moi : l'Arène\n"
	.string "Pokémon d'Azuria.\p"
	.string "Et cette fois, je ne pêchais pas…\n"
	.string "Je t'attendais.$"

RustboroCity_Gym_Text_RoxanneDefeat:
	.string "…\p"
	.string "Pas mal, {PLAYER}.\n"
	.string "Vraiment pas mal…\p"
	.string "Je crois que j'ai sous-estimé ton\n"
	.string "POKéMON.\p"
	.string "Bon. La Ligue Pokémon est claire :\n"
	.string "un champion d'arène qui perd doit\l"
	.string "remettre ceci.\p"
	.string "Tiens. Le BADGE CASCADE de la\n"
	.string "Ligue Pokémon, en bonne et due\l"
	.string "forme.$"

RustboroCity_Gym_Text_ReceivedStoneBadge:
	.string "{PLAYER} a reçu le BADGE CASCADE\n"
	.string "de la part d'ONDINE !$"

RustboroCity_Gym_Text_StoneBadgeInfoTakeThis:
	.string "Le BADGE CASCADE augmente\n"
	.string "l'ATTAQUE de tous tes POKéMON.\p"
	.string "Il leur permet aussi d'utiliser la\n"
	.string "CT COUPE en dehors des combats.\p"
	.string "Tiens, prends aussi ceci.$"

RustboroCity_Gym_Text_ExplainRockTomb:
	.string "Cette Machine Technique, la CT39,\n"
	.string "contient Éboulement.\p"
	.string "Elle inflige des dégâts en faisant\n"
	.string "tomber des rochers, et réduit\l"
	.string "aussi la VITESSE.\p"
	.string "Une CT enseigne la capacité\n"
	.string "immédiatement à un POKéMON.\p"
	.string "Retiens qu'une CT ne s'utilise\n"
	.string "qu'une seule fois, alors réfléchis\l"
	.string "avant de t'en servir.$"

RustboroCity_Gym_Text_RoxannePostBattle:
	.string "Puisque tu es aussi fort, va\n"
	.string "défier d'autres champions\l"
	.string "d'arène.\p"
	.string "En affrontant beaucoup de\n"
	.string "dresseurs, tu apprendras plein\l"
	.string "de choses.$"

RustboroCity_Gym_Text_GymStatue:
	.string "ARÈNE POKéMON D'AZURIA$"

RustboroCity_Gym_Text_GymStatueCertified:
	.string "ARÈNE POKéMON D'AZURIA\p"
	.string "DRESSEURS CERTIFIÉS D'ONDINE :\n"
	.string "{PLAYER}$"

RustboroCity_Gym_Text_RoxanneRegisterCall:
	.string "… … … … … …\n"
	.string "… … … … … Bip !\p"
	.string "ONDINE : Oh, allô, {PLAYER} ?\n"
	.string "C'est ONDINE, d'Azuria.\p"
	.string "BRAWLY m'a dit que tu progressais\n"
	.string "à une vitesse incroyable.\p"
	.string "Bon, je ne vais pas me dire que\n"
	.string "je ne peux pas te battre.\p"
	.string "Je te promets de retravailler mon\n"
	.string "équipe pour que tu puisses me\l"
	.string "redéfier.\p"
	.string "J'espère que tu attends ça avec\n"
	.string "autant d'impatience que moi !\p"
	.string "… … … … … …\n"
	.string "… … … … … Clic !$"

RustboroCity_Gym_Text_RegisteredRoxanne:
	.string "Championne d'arène ONDINE\n"
	.string "enregistrée dans le POKéNAV.$"

RustboroCity_Gym_Text_RoxannePreRematch:
	.string "ROXANNE: I'm so glad to see you again.\n"
	.string "I'm ROXANNE, the GYM LEADER here.\p"
	.string "I'm sure we've both experienced many\n"
	.string "battles since we last met.\p"
	.string "I would like to see how much better\n"
	.string "we've become. Let us battle!$"

RustboroCity_Gym_Text_RoxanneRematchDefeat:
	.string "Grr…\n"
	.string "Again, I have lost…$"

RustboroCity_Gym_Text_RoxannePostRematch:
	.string "ROXANNE: I still have much to learn\n"
	.string "when it comes to battling POKéMON.\p"
	.string "That awareness makes me love battling\n"
	.string "all that much more!$"

RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons:
	.string "ROXANNE: I'm so glad to see you again.\n"
	.string "I'm ROXANNE, the GYM LEADER here.\p"
	.string "I'm sure we've both experienced many\n"
	.string "battles since we last met.\p"
	.string "I would like to see how much better\n"
	.string "we've become…\p"
	.string "Oh, no…\n"
	.string "You only have one POKéMON with you?\p"
	.string "Please return with at least one more\n"
	.string "POKéMON.$"
