PalletTown_RedsHouse_2F_hns_MapScripts::
	.byte 0

PalletTown_RedsHouse_2F_EventScript_NES::
	msgbox RedsHouse_Text_NES, MSGBOX_SIGN
	end

PalletTown_RedsHouse_2F_EventScript_PC::
	msgbox RedsHouse_Text_PC, MSGBOX_SIGN
	end

RedsHouse_Text_NES:
	.string "{PLAYER} a joué à la NES.\p"
	.string "Il vaut mieux y aller, il n'y a pas\n"
	.string "de temps à perdre !$"

RedsHouse_Text_PC:
	.string "On dirait qu'il n'a pas servi\n"
	.string "depuis longtemps…$"
