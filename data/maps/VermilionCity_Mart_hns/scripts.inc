VermilionCity_Mart_hns_MapScripts::
	.byte 0

VermilionCity_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart VermilionCity_Mart_hns_Items
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 3
VermilionCity_Mart_hns_Items::
	.2byte ITEM_POKE_BALL
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	.2byte ITEM_ICE_HEAL
	.2byte ITEM_REPEL
	.2byte ITEM_NONE

VermilionCity_Mart_EventScript_BirdMan::
	msgbox VermilionMart_Text_SuperNerd, MSGBOX_NPC
	end

VermilionCity_Mart_EventScript_Woman::
	msgbox VermilionMart_Text_Beauty, MSGBOX_NPC
	end

VermilionMart_Text_SuperNerd:
	.string "La TEAM ROCKET n'est plus\n"
	.string "présente à KANTO.\p"
	.string "Rien que ça, ça me rend\n"
	.string "heureux.$"

VermilionMart_Text_Beauty:
	.string "Je pense aller faire du\n"
	.string "shopping à SAFFRON.$"
