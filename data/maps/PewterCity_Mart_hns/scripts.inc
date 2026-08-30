PewterCity_Mart_hns_MapScripts::
	.byte 0

PewterCity_Mart_EventScript_Youngster::
	msgbox PewterCity_Mart_Text_Youngster, MSGBOX_NPC
	end

PewterCity_Mart_EventScript_Nerd::
	msgbox PewterCity_Mart_Text_SuperNerd, MSGBOX_NPC
	end

PewterCity_Mart_Text_Youngster:
	.string "Salut ! Regarde mon GYARADOS !\p"
	.string "Je l'ai élevé depuis que c'était\n"
	.string "un MAGIKARP. Il est devenu\n"
	.string "incroyablement fort !$"

PewterCity_Mart_Text_SuperNerd:
	.string "Il y avait une fois un vieil\n"
	.string "homme bizarre qui vendait des\n"
	.string "MAGIKARP.\p"
	.string "Il disait que les MAGIKARP du\n"
	.string "LAKE OF RAGE étaient excellents.$"

