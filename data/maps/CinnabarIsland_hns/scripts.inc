CinnabarIsland_hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CinnabarIsland_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	.byte 0

CinnabarIsland_OnTransition:
	setflag FLAG_VISITED_CINNABAR_ISLAND
	callnative SetTimeBasedEncounters
	compare VAR_TYPE_CHOISI, 0
	goto_if_eq CinnabarIsland_OnTransition_NoAttackYet
	goto_if_set FLAG_ATTAQUE_CINNABAR_LANCEE, CinnabarIsland_OnTransition_NoAttackYet
	setflag FLAG_HIDE_CINNABAR_BLUE
	call HeartSoul_EventScript_CinnabarAttack
CinnabarIsland_OnTransition_NoAttackYet:
	end

@ Heart & Soul : Blaine reste joignable tant que l'attaque n'a pas encore ete declenchee
@ (fenetre tres courte, l'OnTransition ci-dessus la lance des la premiere entree sur la map).
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

