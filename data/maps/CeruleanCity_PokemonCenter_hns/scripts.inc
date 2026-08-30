CeruleanCity_PokemonCenter_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CeruleanCity_PokemonCenter_OnTransition
	.byte 0

CeruleanCity_PokemonCenter_OnTransition::
	setrespawn HEAL_LOCATION_CERULEAN_CITY_HNS
	end

CeruleanCity_PokemonCenter_EventScript_Nerd::
	msgbox CeruleanCity_PokemonCenter1F_Text_SuperNerd, MSGBOX_NPC
	end

CeruleanCity_PokemonCenter_EventScript_Guy::
	msgbox CeruleanCity_PokemonCenter1F_Text_GymGuide, MSGBOX_NPC
	end

CeruleanCity_PokemonCenter1F_Text_SuperNerd:
	.string "Pour les combats, je préfère de\n"
	.string "loin utiliser des POKéMON que\l"
	.string "j'élève, même s'ils sont plus\p"
	.string "faibles que des POKéMON tout\n"
	.string "juste capturés.$"

CeruleanCity_PokemonCenter1F_Text_GymGuide:
	.string "Le MAGNET TRAIN roule à plus de\n"
	.string "550 km/h. Il relie KANTO et JOHTO\l"
	.string "en un rien de temps.\p"
	.string "Il rend vraiment JOHTO accessible.$"
