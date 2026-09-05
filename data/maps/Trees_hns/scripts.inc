Trees_hns_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Trees_OnFrame
	.byte 0

@ Heart & Soul : verrouillage de Johto (4/5). Cette carte est cote Johto d'une connexion de
@ carte (pas un warp discret) vers VermilionCity_hns - impossible de la bloquer par un simple
@ coord_event ponctuel puisque le franchissement se fait par un deplacement continu sur toute
@ la largeur de la bordure. Meme motif deja eprouve que HeartSoul_EventScript_
@ CinnabarVerrouilleeCheck (heart_and_soul_act1.inc) : verification a chaque frame tant que le
@ joueur est sur cette carte, retour immediat cote Kanto si FLAG_BLUE_VAINCU n'est pas pose.
@ VAR_TEMP_0 libre (carte neuve, aucune autre entree ON_FRAME_TABLE ni coord_event ne l'utilise).
@ Point de retour verifie sec par decodage croise map.bin/metatile_attributes.bin de
@ VermilionCity_hns (22,5) - a cote du warp du Centre Pokemon (22,4), sans le chevaucher.
Trees_OnFrame::
	map_script_2 VAR_TEMP_0, 0, Trees_EventScript_JohtoLockedCheck
	.2byte 0

Trees_EventScript_JohtoLockedCheck::
	goto_if_set FLAG_BLUE_VAINCU, Trees_JohtoLocked_Skip
	lockall
	msgbox Trees_Text_JohtoLocked, MSGBOX_DEFAULT
	closemessage
	fadescreenswapbuffers FADE_TO_BLACK
	warp MAP_VERMILION_CITY_HNS, 22, 5
	waitstate
	setvar VAR_TEMP_0, 1
	end
Trees_JohtoLocked_Skip:
	@ Sans ce setvar, la condition resterait vraie a chaque frame et bloquerait tout
	@ deplacement/menu du joueur (meme piege deja documente sur CinnabarIsland_hns).
	setvar VAR_TEMP_0, 1
	end

Trees_Text_JohtoLocked:
	.string "Impossible de passer. Les\n"
	.string "liaisons vers Johto sont\l"
	.string "coupees depuis l'attaque de\l"
	.string "Cinnabar.$"
