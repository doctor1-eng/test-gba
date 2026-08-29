CinnabarIsland_PokemonCenter_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CinnabarIsland_PokemonCenter_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CinnabarIsland_PokemonCenter_OnFrame
	.byte 0

CinnabarIsland_PokemonCenter_OnTransition::
	setrespawn HEAL_LOCATION_CINNABAR_ISLAND_HNS
	end

@ Heart & Soul : le choix du type/equipe est interactif (menus dynmultichoice, msgbox) et ne
@ doit donc pas etre lance depuis ON_TRANSITION (qui tourne pendant le chargement de la carte,
@ avant que l'ecran/les fenetres soient prets - source du blocage constate en test).
@ ON_FRAME_TABLE tourne une fois la carte affichee : c'est le point d'accroche correct pour
@ un evenement au premier chargement (meme motif que AzaleaTown_hns/scripts.inc).
CinnabarIsland_PokemonCenter_OnFrame::
	map_script_2 VAR_TYPE_CHOISI, 0, HeartSoul_EventScript_ChooseType
	.2byte 0

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
