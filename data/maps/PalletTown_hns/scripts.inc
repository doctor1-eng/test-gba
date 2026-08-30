PalletTown_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PalletTown_EventScript_CityEnter
	.byte 0

PalletTown_EventScript_CityEnter::
	setflag FLAG_VISITED_PALLET_TOWN
	end

PalletTown_EventScript_Woman::
	goto_if_unset FLAG_ACTE_1_TERMINE, PalletTown_EventScript_WomanFlavor
	call HeartSoul_EventScript_PalletJeuneDresseur
	end

PalletTown_EventScript_WomanFlavor::
	msgbox PalletTown_Text_Teacher, MSGBOX_NPC
	end
PalletTown_EventScript_Fatman::
	msgbox PalletTown_Text_Fisher, MSGBOX_NPC
	end
PalletTown_EventScript_TownSign::
	msgbox PalletTown_Text_Sign, MSGBOX_SIGN
	end	
PalletTown_EventScript_RedsHouseSign::
	msgbox PalletTown_Text_RedsHouseSign, MSGBOX_SIGN
	end
PalletTown_EventScript_BluesHouseSign::
	msgbox PalletTown_Text_BluesHouseSign, MSGBOX_SIGN
	end	
PalletTown_EventScript_OaksLabSign::
	msgbox PalletTown_Text_OaksLabSign, MSGBOX_SIGN
	end	

PalletTown_Text_Teacher:
	.string "Moi aussi, j'élève des POKéMON.\p"
	.string "Ils me servent de gardes\n"
	.string "personnels.$"

PalletTown_Text_Fisher:
	.string "La technologie, c'est incroyable !\p"
	.string "On peut échanger des POKéMON\n"
	.string "à travers le temps, comme un mail.$"

PalletTown_Text_Sign:
	.string "PALLET TOWN\p"
	.string "Un cadre paisible de paix\n"
	.string "et de pureté$"

PalletTown_Text_RedsHouseSign:
	.string "MAISON DE RED$"

PalletTown_Text_OaksLabSign:
	.string "LABORATOIRE POKéMON\n"
	.string "DU PR OAK$"

PalletTown_Text_BluesHouseSign:
	.string "MAISON DE BLUE$"
