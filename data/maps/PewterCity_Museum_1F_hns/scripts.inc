PewterCity_Museum_1F_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RuinsOfAlph_Lab_OnTransition
	.byte 0

PewterCity_Museum_1F_EventScript_RootFossil::
	finditem ITEM_ROOT_FOSSIL
	setflag FLAG_HIDE_PEWTER_ROOT_FOSSIL
	removeobject LOCALID_MUSEUM_ROOTFOSSIL
	end

PewterCity_Museum_1F_EventScript_ClawFossil::
	finditem ITEM_CLAW_FOSSIL
	setflag FLAG_HIDE_PEWTER_CLAW_FOSSIL
	removeobject LOCALID_MUSEUM_CLAWFOSSIL
	end

PewterCity_Museum_1F_EventScript_Receptionist::
	msgbox PewterCity_Museum_1F_Text_Recptionist, MSGBOX_NPC
	end

PewterCity_Museum_1F_EventScript_FossilWoman::
	goto_if_set FLAG_HIDE_PEWTER_CLAW_FOSSIL, PewterCity_Museum_1F_EventScript_FossilWoman2
	goto_if_set FLAG_HIDE_PEWTER_ROOT_FOSSIL, PewterCity_Museum_1F_EventScript_FossilWoman2
	goto_if_ge VAR_TEMP_2, 1, PewterCity_Museum_1F_EventScript_FossilWoman2
	msgbox PewterCity_Museum_1F_Text_FossilWoman, MSGBOX_NPC
	setvar VAR_TEMP_2, 1
	end

PewterCity_Museum_1F_EventScript_FossilWoman2::
	msgbox PewterCity_Museum_1F_Text_FossilWomanAfter, MSGBOX_NPC
	end

PewterCity_Museum_1F_EventScript_Oldman::
	msgbox PewterCity_Museum_1F_Text_Oldman, MSGBOX_NPC
	end

PewterCity_Museum_1F_EventScript_Bones::
	msgbox PewterCity_Museum_1F_Text_Bones, MSGBOX_SIGN
	end

PewterCity_Museum_1F_Text_Recptionist:
	.string "Bienvenue !\p"
	.string "Pour info, tous les grands\n"
	.string "spécimens ont été transférés\l"
	.string "vers d'autres musées pour\l"
	.string "exposition.\p"
	.string "Donc pour l'instant, l'entrée\n"
	.string "est gratuite !\p"
	.string "Allez-y, je vous en prie.$"

PewterCity_Museum_1F_Text_FossilWoman:
	.string "Vous devez être un dresseur\n"
	.string "aguerri.\p"
	.string "On n'a pas trop l'utilité des\n"
	.string "fossiles sur ce comptoir, là-bas.\p"
	.string "Un homme étrange aux cheveux\n"
	.string "argentés en a déposé un tas ici.\p"
	.string "Prenez-les si ça vous intéresse.$"

PewterCity_Museum_1F_Text_FossilWomanAfter:
	.string "On partage souvent nos\n"
	.string "recherches avec les chercheurs\l"
	.string "de RUINS OF ALPH.\p"
	.string "On partage même des fossiles\n"
	.string "avec eux via le PC !$"

PewterCity_Museum_1F_Text_Oldman:
	.string "Je devrais être reconnaissant\n"
	.string "d'avoir vécu si longtemps.\p"
	.string "Je n'aurais jamais cru voir un\n"
	.string "jour les os d'un dragon !$"

PewterCity_Museum_1F_Text_Bones:
	.string "Ossements d'un ancien POKéMON\n"
	.string "dragon.$"
