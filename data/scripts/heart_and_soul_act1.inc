// Heart & Soul - Acte I : la chute de Cinnabar
// Enchaine directement (goto, meme execution de script) a la fin de
// HeartSoul_EventScript_ChooseTeam_Feu (data/scripts/heart_and_soul_intro.inc), une fois
// l'equipe de depart donnee. Se termine par "end", pas "return" : appele par goto, pas call.

HeartSoul_EventScript_CinnabarAttack::
	lockall
	setflag FLAG_ATTAQUE_CINNABAR_LANCEE
	msgbox HeartSoul_Text_AttaqueDebut, MSGBOX_DEFAULT
	fadescreenswapbuffers FADE_TO_BLACK
	setflag FLAG_HIDE_CINNABAR_BLAINE
	removeobject LOCALID_CINNABAR_BLAINE
	setflag FLAG_BLAINE_DISPARU
	fadescreenswapbuffers FADE_FROM_BLACK
	msgbox HeartSoul_Text_BlaineDisparu, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_RefugiesIntro, MSGBOX_DEFAULT
HeartSoul_CinnabarAttack_RefugiesChoice:
	dynmultipush HeartSoul_Text_RefugiesGuider, 0
	dynmultipush HeartSoul_Text_RefugiesCacher, 1
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, HeartSoul_CinnabarAttack_RefugiesGuider
	case 1, HeartSoul_CinnabarAttack_RefugiesCacher
	goto HeartSoul_CinnabarAttack_RefugiesChoice
HeartSoul_CinnabarAttack_RefugiesGuider:
	setflag FLAG_REFUGIES_GUIDES
	addvar VAR_REPUTATION, 2
	msgbox HeartSoul_Text_RefugiesGuiderResultat, MSGBOX_DEFAULT
	goto HeartSoul_CinnabarAttack_Verrouillage
HeartSoul_CinnabarAttack_RefugiesCacher:
	setflag FLAG_REFUGIES_CACHES
	addvar VAR_REPUTATION, 1
	msgbox HeartSoul_Text_RefugiesCacherResultat, MSGBOX_DEFAULT
HeartSoul_CinnabarAttack_Verrouillage:
	setflag FLAG_CINNABAR_VERROUILLEE
	setflag FLAG_ACTE_1_TERMINE
	setflag FLAG_OBJET_PERDU_TROUVE
	msgbox HeartSoul_Text_ObjetTrouve, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_FuiteRoute21, MSGBOX_DEFAULT
	closemessage
	fadescreenswapbuffers FADE_TO_BLACK
	warp MAP_PALLET_TOWN_HNS, 6, 8
	waitstate
	end

HeartSoul_Text_AttaqueDebut:
	.string "Un grondement secoue Cinnabar.\p"
	.string "Des silhouettes en uniforme noir\n"
	.string "debarquent sur le port, la TEAM\l"
	.string "ROCKET.\p"
	.string "Les liaisons avec le reste de\n"
	.string "Kanto viennent d'etre coupees.$"

HeartSoul_Text_BlaineDisparu:
	.string "Dans la confusion, BLAINE a\n"
	.string "disparu.\p"
	.string "Capture ? En fuite ? Impossible\n"
	.string "de le savoir pour l'instant.$"

HeartSoul_Text_RefugiesIntro:
	.string "En te frayant un chemin vers le\n"
	.string "port, tu croises des habitants\l"
	.string "caches dans les grottes du\l"
	.string "MONT CINNABAR.\p"
	.string "Ils te supplient de decider pour\n"
	.string "eux.$"

HeartSoul_Text_RefugiesGuider:
	.string "Les guider vers un bateau$"

HeartSoul_Text_RefugiesCacher:
	.string "Les cacher sur place$"

HeartSoul_Text_RefugiesGuiderResultat:
	.string "Tu guides le petit groupe jusqu'a\n"
	.string "un bateau de fortune, au risque\l"
	.string "d'etre reperes par la TEAM\l"
	.string "ROCKET.\p"
	.string "Ils s'eloignent du port, vivants.$"

HeartSoul_Text_RefugiesCacherResultat:
	.string "Tu les caches du mieux que tu\n"
	.string "peux dans les grottes.\p"
	.string "Pas de garantie qu'ils soient\n"
	.string "toujours la a ton retour…$"

HeartSoul_Text_FuiteRoute21:
	.string "Il n'y a plus rien a faire ici.\p"
	.string "Cinnabar est coupee du monde. Un\n"
	.string "dernier bateau attend au port,\l"
	.string "pret a partir vers le continent.$"

HeartSoul_Text_ObjetTrouve:
	.string "En courant vers le port, tu\n"
	.string "trebuches sur un bracelet grave,\l"
	.string "tombe des affaires d'une famille\l"
	.string "de refugies.\p"
	.string "Tu le glisses dans ta poche. Il\n"
	.string "faudra le rendre, si tu croises\l"
	.string "quelqu'un qui le reconnait.$"

// Blocage physique du retour a Cinnabar (FLAG_CINNABAR_VERROUILLEE). Declenche depuis
// CinnabarIsland_hns_MapScripts (data/maps/CinnabarIsland_hns/scripts.inc), via
// MAP_SCRIPT_ON_FRAME_TABLE + map_script_2 (pas ON_TRANSITION : meme regle que le blocage
// deja documente sur CinnabarIsland_PokemonCenter_hns, un menu/msgbox interactif ne peut pas
// tourner avant que l'ecran soit pret). Renvoie le joueur vers le point d'arrivee verifie sur
// le continent (voir HeartSoul_CinnabarAttack_Verrouillage plus haut) plutot que de bloquer
// silencieusement son deplacement.
HeartSoul_EventScript_CinnabarVerrouilleeCheck::
	goto_if_unset FLAG_CINNABAR_VERROUILLEE, HeartSoul_CinnabarVerrouillee_Skip
	goto_if_set FLAG_ACTE_5_DEBLOQUE, HeartSoul_CinnabarVerrouillee_Skip
	lockall
	msgbox HeartSoul_Text_CinnabarToujoursVerrouillee, MSGBOX_DEFAULT
	closemessage
	fadescreenswapbuffers FADE_TO_BLACK
	warp MAP_PALLET_TOWN_HNS, 6, 8
	waitstate
	setvar VAR_TEMP_0, 1
	end
HeartSoul_CinnabarVerrouillee_Skip:
	@ Sans ce setvar, la condition VAR_TEMP_0==0 de CinnabarIsland_hns_MapScripts (ON_FRAME_TABLE)
	@ reste vraie a chaque frame : TryRunOnFrameMapScript() (src/script.c) renvoie alors TRUE en
	@ boucle et ProcessPlayerFieldInput() (src/field_control_avatar.c) ne traite plus jamais le
	@ deplacement ni le bouton START tant que le joueur reste sur cette carte - vu comme un
	@ freeze total. Meme motif que CeruleanCity_Gym_EventScript_TryMachinePart (qui, lui, pose
	@ bien ce setvar sur chaque sortie) : VAR_TEMP_0 est remis a 0 par le moteur a chaque nouveau
	@ chargement de carte (ClearTempFieldEventData, src/event_data.c), donc ce setvar ne fait que
	@ desarmer la verification avant le prochain chargement, sans changer son resultat.
	setvar VAR_TEMP_0, 1
	end

HeartSoul_Text_CinnabarToujoursVerrouillee:
	.string "La TEAM ROCKET tient toujours le\n"
	.string "port. Impossible d'accoster ici\l"
	.string "pour l'instant.$"
