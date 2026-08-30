Route6_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route6_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	.byte 0

Route6_OnTransition:
	callnative SetTimeBasedEncounters
	end

Route6_EventScript_Rex::
	trainerbattle_single TRAINER_REX_HNS, PokefanM_Text_RexSeen, PokefanM_Text_RexBeaten
	msgbox PokefanM_Text_RexAfter, MSGBOX_AUTOCLOSE
	end

Route6_EventScript_Allan::
	trainerbattle_single TRAINER_ALLAN_HNS, PokefanM_Text_AllanSeen, PokefanM_Text_AllanBeaten
	msgbox PokefanM_Text_AllanAfter, MSGBOX_AUTOCLOSE
	end

Route6_EventScript_Pokefan::
	msgbox Route6_Text_PokefanM, MSGBOX_NPC
	end

Route6_EventScript_Sign::
	msgbox Route6_Text_UndergroundPathSign, MSGBOX_SIGN
	end

Route6_EventScript_Item_Silver_Wind::
    finditem ITEM_TM_SILVER_WIND
    end

Route6_Text_PokefanM:
	.string "La route est fermée tant que le\n"
	.string "problème à la POWER PLANT n'est\l"
	.string "pas réglé.$"

Route6_Text_UndergroundPathSign:
	.string "PASSAGE SOUTERRAIN\p"
	.string "CERULEAN CITY - VERMILION CITY$"

PokefanM_Text_RexSeen:
	.string "Mon STANTLER est le plus\n"
	.string "mignon du monde.$"

PokefanM_Text_RexBeaten:
	.string "Mon STANTLER!$"

PokefanM_Text_RexAfter:
	.string "Regarde comme mon STANTLER\n"
	.string "est adorable!\p"
	.string "Il est assez craquant pour te\n"
	.string "faire fondre le cœur, non?$"

PokefanM_Text_AllanSeen:
	.string "Mon GRANBULL est le plus\n"
	.string "mignon du monde.$"

PokefanM_Text_AllanBeaten:
	.string "Mon GRANBULL!$"

PokefanM_Text_AllanAfter:
	.string "Regarde comme mon GRANBULL\n"
	.string "est adorable!\p"
	.string "Il est assez craquant pour te\n"
	.string "faire fondre le cœur, non?$"

