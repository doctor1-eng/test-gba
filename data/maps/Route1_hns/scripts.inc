Route1_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route1_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	.byte 0

Route1_OnTransition: 
	callnative SetTimeBasedEncounters
	end

Route1_EventScript_SchoolboyDanny::
	trainerbattle_single TRAINER_DANNY_HNS, Route1_Text_SchoolboyDanny_Seen, Route1_Text_SchoolboyDanny_Beaten
	msgbox Route1_Text_SchoolboyDanny_AfterBattle, MSGBOX_AUTOCLOSE
	end

Route1_EventScript_Quinn::
	trainerbattle_single TRAINER_QUINN_HNS, Route1_Text_CooltrainerFQuinn_Seen, Route1_Text_CooltrainerFQuinn_Beaten, Route1_EventScript_QuinnPostBattle
	msgbox Route1_Text_CooltrainerFQuinn_AfterBattle, MSGBOX_AUTOCLOSE
	end

// Heart & Soul : trainerbattle_single ne fait PAS tomber l'execution sur les lignes qui
// suivent apres une premiere victoire (uniquement au rappel, une fois le dresseur deja
// vaincu - EventScript_TryDoNormalTrainerBattle/EventScript_EndTrainerBattle utilisent
// gotobeatenscript, pas gotopostbattlescript, sur le chemin de premiere victoire ; voir
// data/scripts/trainer_battle.inc). D'ou le bug remonte ("il faut lui parler apres le
// combat") : sans le 4e argument event_script, HeartSoul_EventScript_MiliceRoute1 n'etait
// jamais atteint a la premiere victoire. Corrige en passant ce script en event_script, meme
// motif que Route2_EventScript_RegisterRob (data/maps/Route2_hns/scripts.inc).
Route1_EventScript_QuinnPostBattle::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox Route1_Text_CooltrainerFQuinn_AfterBattle, MSGBOX_DEFAULT
	call HeartSoul_EventScript_MiliceRoute1
	closemessage
	release
	end

Route1_EventScript_Sign::
	msgbox Route1_Text_Sign, MSGBOX_SIGN
	end

Route1_Text_SchoolboyDanny_Seen:
	.string "Quand des dresseurs se croisent,\n"
	.string "la première chose à faire, c'est\l"
	.string "de se battre.$"

Route1_Text_SchoolboyDanny_Beaten:
	.string "Aïe… j'ai un bilan négatif…$"

Route1_Text_SchoolboyDanny_AfterBattle:
	.string "Pour des dresseurs, c'est normal\n"
	.string "de se battre à chaque rencontre.$"

Route1_Text_CooltrainerFQuinn_Seen:
	.string "Hé, toi ! Envie de te battre ?$"

Route1_Text_CooltrainerFQuinn_Beaten:
	.string "Anéantie…$"

Route1_Text_CooltrainerFQuinn_AfterBattle:
	.string "Tu es forte.\p"
	.string "Ça se voit que tu t'es\n"
	.string "entraînée dur.$"

Route1_Text_Sign:
	.string "ROUTE 1\p"
	.string "PALLET TOWN - VIRIDIAN CITY$"
