CinnabarIsland_BoatCabin_hns_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CinnabarIsland_BoatCabin_OnFrame
	.byte 0

@ Heart & Soul : scene de la tempete/du marin (fin de l'Acte I, heart_and_soul_act1.inc).
@ ON_FRAME_TABLE, pas ON_TRANSITION - meme regle deja eprouvee sur CinnabarIsland_hns et
@ Route21_hns (un lockall/msgbox ne peut pas tourner avant que l'ecran soit pret). Aucune autre
@ entree ON_FRAME_TABLE sur cette carte (nouvelle carte), donc VAR_TEMP_0 utilisable directement
@ sans conflit.
CinnabarIsland_BoatCabin_OnFrame::
	map_script_2 VAR_TEMP_0, 0, HeartSoul_EventScript_TempeteBoatCabinCheck
	.2byte 0

@ Placeholder requis par le format object_event (script jamais reellement declenche : l'objet
@ est verrouille sous lockall pendant toute sa fenetre de visibilite).
CinnabarIsland_BoatCabin_EventScript_MarinUnused::
	end
