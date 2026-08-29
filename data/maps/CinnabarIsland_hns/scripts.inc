CinnabarIsland_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CinnabarIsland_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CinnabarIsland_OnFrame
	.byte 0

CinnabarIsland_OnTransition:
	setflag FLAG_VISITED_CINNABAR_ISLAND
	callnative SetTimeBasedEncounters
	setflag FLAG_HIDE_CINNABAR_BLUE
	end

@ Heart & Soul : blocage physique du retour a Cinnabar tant que FLAG_CINNABAR_VERROUILLEE est
@ pose (voir HeartSoul_EventScript_CinnabarVerrouilleeCheck, data/scripts/heart_and_soul_act1.inc).
@ ON_FRAME_TABLE et pas ON_TRANSITION : meme regle que ci-dessus, un msgbox/lockall ne peut pas
@ tourner avant que l'ecran soit pret. VAR_TEMP_0, 0 est une condition toujours vraie (meme motif
@ que CeruleanCity_Gym_hns/scripts.inc, CeruleanCity_Gym_OnFrame) : le script appele decide
@ lui-meme, via goto_if_set/unset sur les vrais flags, s'il doit agir ou ne rien faire.
CinnabarIsland_OnFrame::
	map_script_2 VAR_TEMP_0, 0, HeartSoul_EventScript_CinnabarVerrouilleeCheck
	.2byte 0

@ Heart & Soul : l'attaque (HeartSoul_EventScript_CinnabarAttack, dans
@ data/scripts/heart_and_soul_act1.inc) est enchainee directement depuis
@ HeartSoul_EventScript_ChooseTeam_Feu (meme execution de script, pas de rechargement de
@ carte entre les deux) - pas besoin de la redeclencher ici. Un ancien essai la relancait
@ depuis CinnabarIsland_OnTransition (MAP_SCRIPT_ON_TRANSITION) ; retire, car ce point
@ d'accroche tourne avant que l'ecran soit pret pour des fenetres/menus (voir le blocage
@ constate en test sur le meme probleme dans CinnabarIsland_PokemonCenter_hns).
@ Blaine reste joignable tant que l'attaque n'a pas encore ete declenchee.
CinnabarIsland_EventScript_Blaine::
	lock
	faceplayer
	msgbox CinnabarIsland_Text_Blaine, MSGBOX_DEFAULT
	release
	end

CinnabarIsland_Text_Blaine:
	.string "BLAINE: Tout est calme, pour\n"
	.string "l'instant…\p"
	.string "Mais je n'aime pas ce silence.$"

@ Heart & Soul : Blue n'est plus un ancien Champion de passage a Cinnabar.
@ Il est masque des l'arrivee sur la map (voir CinnabarIsland_OnTransition) : il dirige
@ l'attaque depuis ailleurs, il ne reste jamais visible ici. Ce script ne devrait plus
@ jamais se declencher, mais on le remplace quand meme pour ne rien laisser d'incoherent.
CinnabarIsland_EventScript_Blue::
	lock
	faceplayer
	release
	end

CinnabarIsland_EventScript_Sign::
	msgbox CinnabarIsland_Text_Sign, MSGBOX_SIGN
	end

CinnabarIsland_EventScript_GymSign::
	msgbox CinnabarIsland_Text_GymSign, MSGBOX_SIGN
	end

CinnabarIsland_Text_GymSign:
	.string "There's a notice here…\p"
	.string "CINNABAR GYM has relocated to\n"
	.string "SEAFOAM ISLANDS.\p"
	.string "BLAINE$"

CinnabarIsland_Text_Sign:
	.string "CINNABAR ISLAND\p"
	.string "The Fiery Town of Burning Desire$"

