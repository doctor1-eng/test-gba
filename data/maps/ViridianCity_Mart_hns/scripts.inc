ViridianCity_Mart_hns_MapScripts::
	.byte 0

ViridianCity_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart ViridianCity_Mart_hns_Items
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 3
ViridianCity_Mart_hns_Items::
	.2byte ITEM_POKE_BALL
	.2byte ITEM_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_NONE

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
	.string "Le Champion d'ici est vraiment cool.$"

ViridianMart_Text_CooltrainerM:
	.string "Tu es déjà allé à CINNABAR ?\p"
	.string "C'est une île loin au sud d'ici.$"

