CinnabarIsland_Cave_Hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CinnabarIslandCave_Hns_OnTransition
	.byte 0

@ Heart & Soul - garde-fou de chargement : les 2 habitants (LOCALID_CINNABAR_CAVE_HABITANT1/2)
@ ne doivent etre visibles que si (a) le choix refugies de l'Acte I a deja ete fait ET (b) le
@ joueur a choisi de les cacher (FLAG_REFUGIES_CACHES), pas de les guider vers le bateau
@ (FLAG_REFUGIES_GUIDES - dans ce cas ils sont deja partis, la grotte est vide).
@ Reevalue a CHAQUE chargement de la carte (pas un setflag one-shot) : necessaire pour rester
@ correct meme si la carte est visitee dans le mauvais ordre (avant l'Acte I, ou apres un choix
@ "guider" puis un retour en arriere) - un simple setflag conditionnel figerait l'etat "cache"
@ a la premiere visite prematuree et ne se corrigerait plus jamais ensuite.
CinnabarIslandCave_Hns_OnTransition:
	goto_if_unset FLAG_ACTE_1_TERMINE, CinnabarIslandCave_Hns_ForceHideHabitants
	goto_if_unset FLAG_REFUGIES_CACHES, CinnabarIslandCave_Hns_ForceHideHabitants
	clearflag FLAG_HIDE_CINNABAR_CAVE_HABITANTS
	end
CinnabarIslandCave_Hns_ForceHideHabitants:
	setflag FLAG_HIDE_CINNABAR_CAVE_HABITANTS
	end
