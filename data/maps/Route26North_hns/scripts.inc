Route26North_hns_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route26North_OnFrame
	.byte 0

@ Heart & Soul : verrouillage de Johto (5/5). Meme motif que Trees_hns (connexion de carte vers
@ Route22_hns, pas un warp discret). VAR_TEMP_1 utilise et non VAR_TEMP_0 : cette carte a deja
@ un coord_event trigger existant (Route26North_Trigger, plus bas) qui utilise VAR_TEMP_0 et le
@ met a 1 des que le joueur marche sur (12,6) - reutiliser VAR_TEMP_0 ici desynchroniserait les
@ deux systemes (coord_event vs ON_FRAME_TABLE partagent le meme espace de variables globales,
@ meme si les mecanismes qui les evaluent sont distincts). Point de retour verifie sec sur
@ Route22_hns (12,10), a l'ecart du warp existant vers ReceptionGate_hns (12,9).
Route26North_OnFrame::
	map_script_2 VAR_TEMP_1, 0, Route26North_EventScript_JohtoLockedCheck
	.2byte 0

Route26North_EventScript_JohtoLockedCheck::
	goto_if_set FLAG_BLUE_VAINCU, Route26North_JohtoLocked_Skip
	lockall
	msgbox Route26North_Text_JohtoLocked, MSGBOX_DEFAULT
	closemessage
	fadescreenswapbuffers FADE_TO_BLACK
	warp MAP_ROUTE22_HNS, 12, 10
	waitstate
	setvar VAR_TEMP_1, 1
	end
Route26North_JohtoLocked_Skip:
	setvar VAR_TEMP_1, 1
	end

Route26North_Text_JohtoLocked:
	.string "Impossible de passer. Les\n"
	.string "liaisons vers Johto sont\l"
	.string "coupées depuis l'attaque de\l"
	.string "Cinnabar.$"

Route26North_Trigger::
	setflag FLAG_VISITED_RECEPTION_GATE
	setvar VAR_TEMP_0, 1
	end
