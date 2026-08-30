ViridianCity_PokemonCenter_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ViridianCity_PokemonCenter_OnTransition
	.byte 0

ViridianCity_PokemonCenter_OnTransition::
	setrespawn HEAL_LOCATION_VIRIDIAN_CITY_HNS
	end

ViridianCity_PokemonCenter_EventScript_CooltrainerM::
	goto_if_set FLAG_HIDE_CINNABAR_BLUE, VirianCity_EventScript_GymGrampsBlue
	msgbox ViridianCity_PokemonCenter_Text_1FCooltrainerM, MSGBOX_NPC
	end

ViridianCity_PokemonCenter_EventScript_CooltrainerMBlue::
	msgbox ViridianCity_PokemonCenter_Text_1FCooltrainerM_BlueReturned, MSGBOX_NPC
	end

ViridianCity_PokemonCenter_EventScript_CooltrainerF::
	msgbox ViridianCity_PokemonCenter_Text_1FCooltrainerF, MSGBOX_NPC
	end

ViridianCity_PokemonCenter_EventScript_BugCatcher::
	msgbox ViridianCity_PokemonCenter_Text_1FBugCatcher, MSGBOX_NPC
	end

ViridianCity_PokemonCenter_Text_1FCooltrainerM:
	.string "Où diable est passé le Champion\n"
	.string "de VIRIDIAN ? Je voulais le\l"
	.string "défier.$"

ViridianCity_PokemonCenter_Text_1FCooltrainerM_BlueReturned:
	.string "Il n'y a aucun dresseur d'ARENE\n"
	.string "à VIRIDIAN.\p"
	.string "Le Champion affirme que sa\n"
	.string "politique est de gagner sans avoir\l"
	.string "de subordonnés.$"

ViridianCity_PokemonCenter_Text_1FCooltrainerF:
	.string "J'ai entendu dire que l'ARENE de\n"
	.string "CINNABAR a disparu.\p"
	.string "Je me demande ce qu'est devenu\n"
	.string "BLAINE, le Champion.$"

ViridianCity_PokemonCenter_Text_1FBugCatcher:
	.string "Mon rêve, c'est de devenir\n"
	.string "Champion d'ARENE.$"
