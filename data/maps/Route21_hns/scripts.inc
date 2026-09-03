Route21_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route21_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route21_OnFrame
	.byte 0

Route21_OnTransition:
	call HeartSoul_EventScript_CheckAel
	call HeartSoul_EventScript_CheckOrin
	callnative SetTimeBasedEncounters
	end

@ Heart & Soul : scene de la tempete/du marin (fin de l'Acte I, heart_and_soul_act1.inc),
@ declenchee depuis MAP_SCRIPT_ON_FRAME_TABLE plutot que ON_TRANSITION - meme regle deja
@ eprouvee sur CinnabarIsland_hns (un lockall/msgbox ne peut pas tourner avant que l'ecran soit
@ pret). VAR_TEMP_0 : aucune autre entree ON_FRAME_TABLE n'existait sur cette carte avant cet
@ ajout, donc pas de conflit de plage comme celui rencontre sur CinnabarIsland_hns (qui a
@ necessite VAR_TEMP_1 a cause d'une entree VAR_TEMP_0 deja existante).
Route21_OnFrame::
	map_script_2 VAR_TEMP_0, 0, HeartSoul_EventScript_TempeteRoute21Check
	.2byte 0

Route21_EventScript_Seth::
	trainerbattle_single TRAINER_SETH_HNS, Route21_Text_SwimmermSethSeen, Route21_Text_SwimmermSethBeaten
	msgbox Route21_Text_SwimmermSethAfterBattle, MSGBOX_AUTOCLOSE
	end

Route21_EventScript_Nikki::
	trainerbattle_single TRAINER_NIKKI_HNS, Route21_Text_SwimmerfNikkiSeen, Route21_Text_SwimmerfNikkiBeaten
	msgbox Route21_Text_SwimmerfNikkiAfterBattle, MSGBOX_AUTOCLOSE
	end

Route21_EventScript_Arnold::
	trainerbattle_single TRAINER_ARNOLD_HNS, Route21_Text_FisherArnoldSeen, Route21_Text_FisherArnoldBeaten
	msgbox Route21_Text_FisherArnoldAfterBattle, MSGBOX_AUTOCLOSE
	end

Route21_Text_SwimmermSethSeen:
	.string "Terre en vue ! Je dois continuer !$"

Route21_Text_SwimmermSethBeaten:
	.string "Glup…$"

Route21_Text_SwimmermSethAfterBattle:
	.string "Ce type arrogant était au volcan\n"
	.string "de CINNABAR.$"

Route21_Text_SwimmerfNikkiSeen:
	.string "Si je gagne, tu devras m'aider à\n"
	.string "mettre ma crème solaire !$"

Route21_Text_SwimmerfNikkiBeaten:
	.string "J'ai peur des coups de soleil…$"

Route21_Text_SwimmerfNikkiAfterBattle:
	.string "Je dois faire attention aux taches\n"
	.string "causées par le soleil.$"

Route21_Text_FisherArnoldSeen:
	.string "La pêche m'ennuie. On se bat ?$"

Route21_Text_FisherArnoldBeaten:
	.string "Échec total…$"

Route21_Text_FisherArnoldAfterBattle:
	.string "Je vais juste retourner pêcher…$"

