VermilionCity_PokemonCenter_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VermilionCity_PokemonCenter_OnTransition
	.byte 0

VermilionCity_PokemonCenter_OnTransition::
	setrespawn HEAL_LOCATION_VERMILION_CITY_HNS
	end

VermilionPokeCenter_EventScript_FishingGuru::
	goto_if_set FLAG_HIDE_VERMILION_SNORLAX, VermilionPokeCenter_EventScript_FishingGuru2
	msgbox VermilionCity_PokemonCenter1F_Text_FishingGuru, MSGBOX_NPC
	end

VermilionPokeCenter_EventScript_FishingGuru2::
	msgbox VermilionCity_PokemonCenter1F_Text_FishingGuru_FoughtSnorlax, MSGBOX_NPC
	end

VermilionPokeCenter_EventScript_Sailor::
	msgbox VermilionCity_PokemonCenter1F_Text_Sailor, MSGBOX_NPC
	end

VermilionPokeCenter_EventScript_Bugcatcher::
	msgbox VermilionCity_PokemonCenter1F_Text_BugCatcher, MSGBOX_NPC
	end

VermilionCity_PokemonCenter1F_Text_FishingGuru:
	.string "Un POKéMON endormi est allongé\n"
	.string "devant DIGLETT'S CAVE.\p"
	.string "C'est une occasion en or de\n"
	.string "l'attraper, mais comment le\l"
	.string "réveiller?$"

VermilionCity_PokemonCenter1F_Text_FishingGuru_FoughtSnorlax:
	.string "Il y avait un POKéMON endormi\n"
	.string "allongé devant DIGLETT'S CAVE.\p"
	.string "Mais il semble avoir disparu.$"

VermilionCity_PokemonCenter1F_Text_Sailor:
	.string "Le FAST SHIP est un endroit\n"
	.string "génial pour rencontrer et\l"
	.string "affronter des dresseurs.$"

VermilionCity_PokemonCenter1F_Text_BugCatcher:
	.string "Oh? Tu as des BADGES que je\n"
	.string "n'ai jamais vus.\p"
	.string "Ah, je vois. Tu les as eus à\n"
	.string "JOHTO.$"
