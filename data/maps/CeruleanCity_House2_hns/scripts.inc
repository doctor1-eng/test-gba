CeruleanCity_House2_hns_MapScripts::
	.byte 0

CeruleanCity_House2_EventScript_Woman::
	msgbox CeruleanPolice_Text_PokefanF, MSGBOX_NPC
	end

CeruleanCity_House2_EventScript_Man::
	msgbox CeruleanPolice_Text_FishingGuru, MSGBOX_NPC
	end

CeruleanCity_House2_EventScript_Diglett::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_DIGLETT, CRY_MODE_NORMAL
	msgbox CeruleanPolice_Text_Diglett, MSGBOX_DEFAULT
	waitmoncry
	release
	end

CeruleanPolice_Text_FishingGuru:
	.string "J'ai entendu dire qu'un type\n"
	.string "louche rôdait dans le coin.\p"
	.string "Je ne tolérerai pas que ce soit\n"
	.string "un voleur.$"

CeruleanPolice_Text_PokefanF:
	.string "On s'est déjà fait attaquer par\n"
	.string "des voleurs.$"

CeruleanPolice_Text_Diglett:
	.string "DIGLETT: Dug dug.$"
