PalletTown_RedsHouse_1F_hns_MapScripts::
	.byte 0

PalletTown_RedsHouse_1F_EventScript_Mom::
	lock
	faceplayer
	goto_if_ge VAR_TEMP_0, 1, PalletTown_RedsHouse_1F_EventScript_MomAfter
	msgbox RedsHouse_Text_Mom1, MSGBOX_DEFAULT
	closemessage
	setvar VAR_TEMP_0, 1
	release
	end

PalletTown_RedsHouse_1F_EventScript_MomAfter::
	msgbox RedsHouse_Text_Mom2, MSGBOX_DEFAULT
	closemessage
	release
	end

PalletTown_RedsHouse_1F_EventScript_TV::
	msgbox RedsHouse_Text_TV, MSGBOX_SIGN
	end

RedsHouse_Text_Mom1:
	.string "Salut !\p"
	.string "Ça fait longtemps que RED est parti.\p"
	.string "Il n'a même pas appelé, alors je\n"
	.string "n'ai aucune idée d'où il est ni de\l"
	.string "ce qu'il fait.\p"
	.string "On dit que pas de nouvelles, bonnes\n"
	.string "nouvelles, mais je m'inquiète quand\l"
	.string "même pour lui.$"

RedsHouse_Text_Mom2:
	.string "Je m'inquiète que RED se blesse ou\n"
	.string "tombe malade, mais c'est un garçon.\p"
	.string "Je suis fière qu'il fasse ce qu'il\n"
	.string "veut faire.$"

RedsHouse_Text_TV:
	.string "Ils ont des émissions qu'on ne\n"
	.string "voit pas dans JOHTO…$"
