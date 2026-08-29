CinnabarIsland_PokemonCenter_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CinnabarIsland_PokemonCenter_OnTransition
	.byte 0

CinnabarIsland_PokemonCenter_OnTransition::
	setrespawn HEAL_LOCATION_CINNABAR_ISLAND_HNS
	compare VAR_TYPE_CHOISI, 0
	goto_if_ne CinnabarIsland_PokemonCenter_OnTransition_TypeAlreadyChosen
	goto HeartSoul_EventScript_ChooseType
CinnabarIsland_PokemonCenter_OnTransition_TypeAlreadyChosen:
	end

CinnabarIsland_PokemonCenter_EventScript_Cooltrainer::
	msgbox CinnabarIsland_PokemonCenter1F_Text_CooltrainerF, MSGBOX_NPC
	end

CinnabarIsland_PokemonCenter_EventScript_Fisher::
	msgbox CinnabarIsland_PokemonCenter1F_Text_Fisher, MSGBOX_NPC
	end

CinnabarIsland_PokemonCenter1F_Text_CooltrainerF:
	.string "On dit que BLAINE veut rebatir\n"
	.string "l'ARENE depuis des annees…$"

CinnabarIsland_PokemonCenter1F_Text_Fisher:
	.string "Cinnabar a toujours ete isolee.\n"
	.string "On s'en sort seuls, ici.$"
