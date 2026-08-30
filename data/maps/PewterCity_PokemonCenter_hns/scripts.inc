PewterCity_PokemonCenter_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PewterCity_PokemonCenter_OnTransition
	.byte 0

PewterCity_PokemonCenter_OnTransition::
	setrespawn HEAL_LOCATION_PEWTER_CITY_HNS
	end

PewterCity_PokemonCenter_EventScript_Teacher::
	msgbox PewterCity_PokemonCenter1F_Text_Teacher, MSGBOX_NPC
	end

PewterCity_PokemonCenter_EventScript_BugCatcher::
	msgbox PewterCity_PokemonCenter1F_Text_BugCatcher, MSGBOX_NPC
	end

PewterCity_PokemonCenter1F_Text_Teacher:
	.string "…Ouais, et l'arène de CINNABAR\n"
	.string "a disparu.\p"
	.string "J'étais vraiment stupéfait.\p"
	.string "…Oui ? Je suis au téléphone.\n"
	.string "Dégage !$"

PewterCity_PokemonCenter1F_Text_Jigglypuff:
	.string "JIGGLYPUFF: Puu pupuu.$"

PewterCity_PokemonCenter_EventScript_Mondo::
	lock
	faceplayer
	goto_if_set FLAG_PEWTER_NPC_TRADE_COMPLETED, PewterCity_PokemonCenter_EventScript_TradeCompleted
	ingame_trade INGAME_TRADE_HAUNTER, PewterCity_PokemonCenter_Text_WantToTrade, PewterCity_PokemonCenter_EventScript_DeclineTrade, PewterCity_PokemonCenter_EventScript_NotRequestedMon, PewterCity_PokemonCenter_Text_TakeCare
	setflag FLAG_PEWTER_NPC_TRADE_COMPLETED
	release
	end

PewterCity_PokemonCenter_EventScript_DeclineTrade::
	msgbox PewterCity_PokemonCenter_Text_DeclineTrade, MSGBOX_DEFAULT
	release
	end

PewterCity_PokemonCenter_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox PewterCity_PokemonCenter_Text_NotRequestedMon, MSGBOX_DEFAULT
	release
	end

PewterCity_PokemonCenter_EventScript_TradeCompleted::
	msgbox PewterCity_PokemonCenter_Text_TradeCompleted, MSGBOX_DEFAULT
	release
	end

PewterCity_PokemonCenter_Text_WantToTrade:
	.string "J'ai parcouru le pays à la\n"
	.string "recherche d'un bon {STR_VAR_1}.\p"
	.string "Voudrais-tu en échanger un\n"
	.string "contre mon {STR_VAR_2} ?$"

PewterCity_PokemonCenter_Text_TakeCare:
	.string "Merci beaucoup !\n"
	.string "Prends bien soin de mon {STR_VAR_1} !$"

PewterCity_PokemonCenter_Text_DeclineTrade:
	.string "Non ? Tant pis…\n"
	.string "Je pense que tu aimerais mon\l"
	.string "HAUNTER.$"

PewterCity_PokemonCenter_Text_NotRequestedMon:
	.string "Hmm, ce n'est pas un {STR_VAR_1}.\n"
	.string "Ce n'est pas ce que je cherche.$"

PewterCity_PokemonCenter_Text_TradeCompleted:
	.string "Comment se comporte ce HAUNTER ?\n"
	.string "C'est un vrai farceur !$"

PewterCity_PokemonCenter1F_Text_BugCatcher:
	.string "La plupart des POKéMON\n"
	.string "s'endorment en entendant\l"
	.string "chanter un JIGGLYPUFF.\p"
	.string "Il existe plusieurs capacités\n"
	.string "utilisables seulement quand un\l"
	.string "POKéMON dort.$"
