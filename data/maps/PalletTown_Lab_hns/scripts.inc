PalletTown_Lab_hns_MapScripts::
	.byte 0

PalletTown_Lab_EventScript_Oak::
	lock
	faceplayer
	call_if_unset FLAG_SYS_NATIONAL_DEX, PalletTown_Lab_EventScript_OakGiveNatDex_ForPlaytesters
	goto_if_eq VAR_PALLETTOWN_LABSTATE, 1, PalletTown_Lab_EventScript_OakBadgeCheck
	goto_if_eq VAR_PALLETTOWN_LABSTATE, 2, PalletTown_Lab_EventScript_OakDex
	msgbox PalletTown_Lab_Text_OakWelcomeKanto, MSGBOX_DEFAULT
	msgbox PalletTown_Lab_Text_OakLabGoodbye, MSGBOX_DEFAULT
	setvar VAR_PALLETTOWN_LABSTATE, 1
	goto_if_ge VAR_NUM_BADGES, 16, PalletTown_Lab_EventScript_OakBadgeCheck
	closemessage
	release
	end

PalletTown_Lab_EventScript_OakGiveNatDex_ForPlaytesters::
	setflag FLAG_SYS_NATIONAL_DEX
	special EnableNationalPokedex
	return

PalletTown_Lab_EventScript_OakBadgeCheck::
	goto_if_ge VAR_NUM_BADGES, 16, PalletTown_Lab_EventScript_Oak16Badges
	goto_if_ge VAR_NUM_BADGES, 9, PalletTown_Lab_EventScript_Oak9Badges
	msgbox PalletTown_Lab_Text_OakNoKantoBadges, MSGBOX_DEFAULT
	closemessage
	release
	end

PalletTown_Lab_EventScript_Oak16Badges::
	msgbox PalletTown_Lab_Text_OakOpenMtSilver, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_INDIGOJUNCTION_HIDE_SILVER_GUARD
	setvar VAR_PALLETTOWN_LABSTATE, 2
	call_if_unset FLAG_IS_KANTO_CHAMPION, PalletTown_Lab_EventScript_OakKantoLeague
	release
	end

PalletTown_Lab_EventScript_OakKantoLeague::
	msgbox PalletTown_Lab_Text_OakOpenIndigoPlateau, MSGBOX_DEFAULT
	closemessage
	return

PalletTown_Lab_EventScript_Oak9Badges::
	msgbox PalletTown_Lab_Text_OakYesKantoBadges, MSGBOX_DEFAULT
	closemessage
	release
	end

PalletTown_Lab_EventScript_OakDex::
	msgbox PalletTown_Lab_Text_OakLabDexCheck, MSGBOX_DEFAULT
	closemessage
	release
	end

PalletTown_Lab_EventScript_Assistant1::
	msgbox PalletTown_Lab_Text_OaksAssistant1, MSGBOX_NPC
	end

PalletTown_Lab_EventScript_Assistant2::
	msgbox PalletTown_Lab_Text_OaksAssistant2, MSGBOX_NPC
	end

PalletTown_Lab_EventScript_Assistant3::
	msgbox PalletTown_Lab_Text_OaksAssistant3, MSGBOX_NPC
	end

PalletTown_Lab_EventScript_PC::
	msgbox PalletTown_Lab_Text_OaksLabPC, MSGBOX_SIGN
	end

PalletTown_Lab_Text_OakLabDexCheck:
	.string "Où en est ton POKéDEX ?$"

PalletTown_Lab_Text_OakWelcomeKanto:
	.string "OAK: Ah, {PLAYER} !\n"
	.string "C'est gentil d'être venu de si\l"
	.string "loin jusqu'à KANTO.\p"
	.string "Que penses-tu des dresseurs\n"
	.string "d'ici ? Plutôt costauds, non ?$"

PalletTown_Lab_Text_OakLabGoodbye:
	.string "Si jamais tu repasses dans le\n"
	.string "coin, viens me revoir.$"

PalletTown_Lab_Text_OakOpenMtSilver:
	.string "OAK: Waouh ! C'est excellent !\p"
	.string "Tu as récolté les BADGES des\n"
	.string "ARENES de KANTO. Bien joué !\p"
	.string "J'avais raison à ton sujet.\p"
	.string "Je vais te dire, {PLAYER}. Je vais\n"
	.string "m'arranger pour que tu puisses\l"
	.string "aller à MT.SILVER.\p"
	.string "MT.SILVER est une grande montagne\n"
	.string "où vivent de nombreux POKéMON\l"
	.string "sauvages.\p"
	.string "C'est trop dangereux pour un\n"
	.string "dresseur ordinaire, l'accès est\l"
	.string "donc interdit. Mais on peut faire\l"
	.string "une exception dans ton cas,\l"
	.string "{PLAYER}.\p"
	.string "Monte jusqu'à INDIGO PLATEAU.\n"
	.string "Tu pourras rejoindre MT.SILVER\l"
	.string "depuis là-bas.$"

PalletTown_Lab_Text_OakOpenIndigoPlateau:
	.string "Oh, et en parlant du PLATEAU\n"
	.string "INDIGO, maintenant que tu as\l"
	.string "gagné 16 badges, la LIGUE KANTO\l"
	.string "est de nouveau prête à\l"
	.string "accepter ton défi.\p"
	.string "Mais attention, LANCE et son\n"
	.string "CONSEIL 4 ne te feront pas de\l"
	.string "cadeau cette fois.\p"
	.string "Bonne chance, {PLAYER} !$"

PalletTown_Lab_Text_OakNoKantoBadges:
	.string "OAK: Hmm ? Tu ne collectionnes\n"
	.string "pas les BADGES de KANTO ?\p"
	.string "Les Champions d'Arène de KANTO\n"
	.string "sont aussi coriaces que ceux que\l"
	.string "tu as affrontés à JOHTO.\p"
	.string "Je te recommande de les défier.$"

PalletTown_Lab_Text_OakYesKantoBadges:
	.string "OAK: Ah, tu collectionnes les\n"
	.string "BADGES de KANTO.\p"
	.string "J'imagine que c'est difficile,\n"
	.string "mais l'expérience va sûrement\l"
	.string "t'aider.\p"
	.string "Reviens me voir quand tu les\n"
	.string "auras tous.\p"
	.string "J'aurai un cadeau pour toi.\p"
	.string "Continue tes efforts, {PLAYER} !$"

PalletTown_Lab_Text_OaksAssistant1:
	.string "L'émission radio POKéMON TALK\n"
	.string "du PR n'est pas diffusée ici, à\l"
	.string "KANTO.\p"
	.string "C'est dommage, j'aimerais\n"
	.string "l'écouter.$"

PalletTown_Lab_Text_OaksAssistant2:
	.string "Grâce à ton travail sur le\n"
	.string "POKéDEX, les recherches du PR\l"
	.string "avancent très bien.$"

PalletTown_Lab_Text_OaksAssistant3:
	.string "Ne le répète à personne, mais\n"
	.string "POKéMON TALK du PR OAK n'est\l"
	.string "pas en direct.$"

PalletTown_Lab_Text_OaksLabPC:
	.string "Il y a un e-mail sur le PC.\p"
	.string "…\p"
	.string "PR OAK, où en sont tes\n"
	.string "recherches ?\p"
	.string "Je persévère toujours.\p"
	.string "J'ai entendu dire que {PLAYER}\n"
	.string "se forge une belle réputation.\p"
	.string "Je suis ravi de l'apprendre.\p"
	.string "ELM, de NEW BARK TOWN 8-)$"
