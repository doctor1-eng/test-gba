ViridianCity_House1_hns_MapScripts::
	.byte 0

ViridianCity_House1_EventScript_Pokefan::
	msgbox ViridianCity_House1_Text_PokefanM, MSGBOX_NPC
	end

ViridianCity_House1_EventScript_Lass::
	msgbox ViridianCity_House1_Text_Lass, MSGBOX_NPC
	end


ViridianCity_House1_EventScript_Speary::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_SPEAROW, CRY_MODE_NORMAL
	msgbox ViridianCity_House1_Text_Speary, MSGBOX_DEFAULT
	waitmoncry
	release
	end

ViridianCity_House1_EventScript_Rattey::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_RATTATA, CRY_MODE_NORMAL
	msgbox ViridianCity_House1_Text_Rattey, MSGBOX_DEFAULT
	waitmoncry
	release
	end

ViridianCity_House1_Text_PokefanM:
	.string "Tu réfléchis beaucoup avant de\n"
	.string "nommer tes POKéMON ?\p"
	.string "Leur donner de bons surnoms rend\n"
	.string "les échanges avec les autres plus\l"
	.string "amusants.$"

ViridianCity_House1_Text_Lass:
	.string "C'est SPEARY, un SPEAROW, et\n"
	.string "RATTEY, un RATTATA.$"

ViridianCity_House1_Text_Speary:
	.string "SPEARY: Ch-chun !$"

ViridianCity_House1_Text_Rattey:
	.string "RATTEY: Kikiii !$"

