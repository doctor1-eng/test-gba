CeruleanCity_Mart_hns_MapScripts::
	.byte 0

CeruleanCity_Mart_EventScript_Juggler::
	msgbox CeruleanMart_Text_CooltrainerM, MSGBOX_NPC
	end

CeruleanCity_Mart_EventScript_CooltrainerF::
	msgbox CeruleanMart_Text_CooltrainerF, MSGBOX_NPC
	end

CeruleanMart_Text_CooltrainerM:
	.string "Tu croiseras beaucoup de\n"
	.string "dresseurs sur le chemin du CAPE\l"
	.string "de CERULEAN.\p"
	.string "Ils veulent voir ce qu'ils valent\n"
	.string "face à d'autres dresseurs.$"

CeruleanMart_Text_CooltrainerF:
	.string "ONDINE est à peu près la seule\n"
	.string "personne en ville capable de\l"
	.string "battre les dresseurs du CAPE de\l"
	.string "CERULEAN.$"
