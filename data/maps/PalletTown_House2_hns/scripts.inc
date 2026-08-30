PalletTown_House2_hns_MapScripts::
	.byte 0

PalletTown_House2_EventScript_Daisy::
	lock
	faceplayer
	goto_if_unset FLAG_HIDE_CINNABAR_BLUE, PalletTown_House2_EventScript_Blue
	goto_if_unset FLAG_DAILY_DAISY_GROOMED, PalletTown_House2_EventScript_DaisyGrooming
	msgbox PalletTown_House2_Text_AlreadyGroomed, MSGBOX_DEFAULT
	closemessage
	release
	end

PalletTown_House2_EventScript_Blue::
	msgbox PalletTown_House2_Text_Hello, MSGBOX_DEFAULT
	closemessage
	release
	end

PalletTown_House2_EventScript_DaisyDeclineGrooming::
	msgbox PalletTown_House2_Text_Refused, MSGBOX_DEFAULT
	closemessage
	release
	end
PalletTown_House2_EventScript_Egg::
	msgbox PalletTown_House2_Text_CantGroomEgg, MSGBOX_DEFAULT
	closemessage
	release
	end

PalletTown_House2_EventScript_DaisyGrooming::
	msgbox PalletTown_House2_Text_OfferGrooming, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, FALSE, PalletTown_House2_EventScript_DaisyDeclineGrooming
	msgbox PalletTown_House2_Text_WhichMon, MSGBOX_DEFAULT
	special ChoosePartyMon
	waitstate
	lock
	faceplayer
	bufferpartymonnick STR_VAR_1, VAR_0x8004
	goto_if_ge VAR_0x8004, PARTY_SIZE, PalletTown_House2_EventScript_DaisyDeclineGrooming
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, PalletTown_House2_EventScript_Egg
	goto_if_eq VAR_RESULT, FALSE, PalletTown_House2_EventScript_DaisyDeclineGrooming
	
	msgbox PalletTown_House2_Text_Alright
	closemessage
	fadescreen FADE_TO_BLACK
	playfanfare MUS_HEAL
	waitfanfare
	special HaircutBrother1
	fadescreen FADE_FROM_BLACK
	special BufferMonNickname
	msgbox PalletTown_House2_Text_AllDone
	setflag FLAG_DAILY_DAISY_GROOMED
	release
	end

PalletTown_House2_Text_Hello:
	.string "DAISY: Salut ! Mon petit frère\n"
	.string "est le Champion de VIRIDIAN\l"
	.string "CITY.\p"
	.string "Mais il s'absente si souvent\n"
	.string "que ça pose problème aux\l"
	.string "dresseurs.$"

PalletTown_House2_Text_OfferGrooming:
	.string "DAISY: Salut ! Bien tombé, j'allais\n"
	.string "justement prendre le thé.\p"
	.string "Tu veux te joindre à moi ?\p"
	.string "Oh, tes POKéMON sont un peu sales.\p"
	.string "Tu veux que je toilette l'un d'eux ?$"

PalletTown_House2_Text_WhichMon:
	.string "DAISY: Lequel dois-je toiletter ?$"

PalletTown_House2_Text_Alright:
	.string "DAISY: D'accord, je vais vite le\n"
	.string "rendre tout beau.$"

PalletTown_House2_Text_AllDone:
	.string "DAISY: Et voilà, c'est terminé !\p"
	.string "Tu vois ? {STR_VAR_1} est tout\n"
	.string "beau, non ?\p"
	.string "C'est un si mignon POKéMON.$"

PalletTown_House2_Text_AlreadyGroomed:
	.string "DAISY: Je prends toujours le thé\n"
	.string "à cette heure-ci. Viens donc.$"

PalletTown_House2_Text_Refused:
	.string "DAISY: Tu ne veux pas que j'en\n"
	.string "toilette un ?\p"
	.string "D'accord, on prendra juste le thé.$"

PalletTown_House2_Text_CantGroomEgg:
	.string "DAISY: Oh, désolée. Je ne peux\n"
	.string "vraiment pas toiletter un EGG.$"
