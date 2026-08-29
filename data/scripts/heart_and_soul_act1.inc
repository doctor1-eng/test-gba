// Heart & Soul - Acte I : la chute de Cinnabar
// Declenche automatiquement par CinnabarIsland_hns_MapScripts (CinnabarIsland_OnTransition)
// des que le joueur a choisi son type/equipe (VAR_TYPE_CHOISI != 0) et que l'attaque n'a
// pas encore ete jouee (FLAG_ATTAQUE_CINNABAR_LANCEE non pose).

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
	msgbox HeartSoul_Text_FuiteRoute21, MSGBOX_DEFAULT
	releaseall
	return

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
	.string "Cinnabar est coupee du monde,\n"
	.string "il faut fuir par la ROUTE 21 et\l"
	.string "rejoindre le continent.$"
