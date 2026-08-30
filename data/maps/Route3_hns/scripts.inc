Route3_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route3_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	.byte 0

Route3_OnTransition: 
	callnative SetTimeBasedEncounters
	end

Route3_EventScript_Otis::
	trainerbattle_single TRAINER_OTIS_HNS, Route3_Text_FirebreatherOtis_Seen, Route3_Text_FirebreatherOtis_Beaten
	msgbox Route3_Text_FirebreatherOtis_AfterBattle, MSGBOX_AUTOCLOSE
	end

Route3_EventScript_Warren::
	trainerbattle_single TRAINER_WARREN_HNS, Route3_Text_YoungsterWarren_Seen, Route3_Text_YoungsterWarren_Beaten
	msgbox Route3_Text_YoungsterWarren_AfterBattle, MSGBOX_AUTOCLOSE
	end

Route3_EventScript_Jimmy::
	trainerbattle_single TRAINER_JIMMY_HNS, Route3_Text_YoungsterJimmy_Seen, Route3_Text_YoungsterJimmy_Beaten
	msgbox Route3_Text_YoungsterJimmy_AfterBattle, MSGBOX_AUTOCLOSE
	end

Route3_EventScript_Burt::
	trainerbattle_single TRAINER_BURT_HNS, Route3_Text_FirebreatherBurt_Seen, Route3_Text_FirebreatherBurt_Beaten
	msgbox Route3_Text_FirebreatherBurt_AfterBattle, MSGBOX_AUTOCLOSE
	end

Route3_EventScript_Sign::
	msgbox Route3_Text_MtMoonSquareSign, MSGBOX_SIGN
	end

Route3_Text_FirebreatherOtis_Seen:
	.string "Ah ! Il fait aussi beau que\n"
	.string "d'habitude.$"

Route3_Text_FirebreatherOtis_Beaten:
	.string "Il fait soleil, mais je suis\n"
	.string "trempé…$"

Route3_Text_FirebreatherOtis_AfterBattle:
	.string "Quand il pleut, difficile de\n"
	.string "faire prendre le feu…$"

Route3_Text_YoungsterWarren_Seen:
	.string "Hmmm… je ne sais pas quoi faire…$"

Route3_Text_YoungsterWarren_Beaten:
	.string "Je savais que j'allais perdre…$"

Route3_Text_YoungsterWarren_AfterBattle:
	.string "Tu avais l'air fort.\p"
	.string "J'avais peur de t'affronter…$"

Route3_Text_YoungsterJimmy_Seen:
	.string "Je cours aussi vite que le vent !$"

Route3_Text_YoungsterJimmy_Beaten:
	.string "Soufflé !$"

Route3_Text_YoungsterJimmy_AfterBattle:
	.string "Je porte un short toute l'année.\p"
	.string "C'est ma philosophie mode.$"

Route3_Text_FirebreatherBurt_Seen:
	.string "Approchez, venez voir !$"

Route3_Text_FirebreatherBurt_Beaten:
	.string "Aïe ! Ça brûle !$"

Route3_Text_FirebreatherBurt_AfterBattle:
	.string "Le plus grand cracheur de feu\n"
	.string "de KANTO, c'est moi.\p"
	.string "Mais pas le meilleur dresseur…$"

Route3_Text_MtMoonSquareSign:
	.string "MT.MOON SQUARE\p"
	.string "Il suffit de monter l'escalier.$"

