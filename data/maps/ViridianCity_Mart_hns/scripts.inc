ViridianCity_Mart_hns_MapScripts::
	.byte 0

ViridianCity_Mart_EventScript_Lass::
	msgbox ViridianMart_Text_Lass, MSGBOX_NPC
	end

ViridianCity_Mart_EventScript_Cooltrainer::
	goto_if_unset FLAG_ACTE_1_TERMINE, ViridianCity_Mart_EventScript_CooltrainerFlavor
	goto_if_set FLAG_COMMERCANT_VIRIDIAN_RESOLU, ViridianCity_Mart_EventScript_CooltrainerAfter
	lock
	faceplayer
	call HeartSoul_EventScript_CommercantViridian
	release
	end

ViridianCity_Mart_EventScript_CooltrainerFlavor::
	msgbox ViridianMart_Text_CooltrainerM, MSGBOX_NPC
	end

ViridianCity_Mart_EventScript_CooltrainerAfter::
	msgbox HeartSoul_Text_CommercantApresResolu, MSGBOX_NPC
	end

ViridianMart_Text_Lass:
	.string "The GYM LEADER here is totally cool.$"

ViridianMart_Text_CooltrainerM:
	.string "Have you been to CINNABAR?\p"
	.string "It's an island way south of here.$"

