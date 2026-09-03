// Heart & Soul - histoire.md section 11, evenements narratifs supplementaires qui ne
// s'inserent pas naturellement dans un fichier d'acte existant (nouveaux object_events/
// bg_events plutot que des ajouts a des scenes deja scriptees).

// Evenement 7 (Acte II, Pewter) : panneau invisible (meme mecanisme que les carnets de
// Blaine du Manoir, data/scripts/heart_and_soul_cinnabar_interiors.inc), position (15,10)
// verifiee par decodage de data/layouts/PewterCity_hns/map.bin.
PewterCity_EventScript_PhotoBlaine::
	goto_if_unset FLAG_ACTE_1_TERMINE, PewterCity_PhotoBlaine_Skip
	msgbox HeartSoul_Text_PhotoBlaine, MSGBOX_SIGN
	return
PewterCity_PhotoBlaine_Skip:
	return

HeartSoul_Text_PhotoBlaine:
	.string "Un vieux cliche, glisse entre\n"
	.string "deux etals du marche.\p"
	.string "BLAINE, plus jeune, un bras sur\n"
	.string "l'epaule d'une dresseuse qui te\l"
	.string "ressemble beaucoup.\p"
	.string "Personne ne t'avait jamais dit\n"
	.string "qu'il te connaissait depuis si\l"
	.string "longtemps.$"

// Evenement 8 (Acte III) : embuscade de reconnaissance, obstacle mineur avant les vrais
// lieutenants. TRAINER_PATROUILLE_RECON_HNS deja alloue (opponents_hns.h). Place sur
// Route3_hns (entre Jadielle et le Mont Selenite), position verifiee par decodage collision.
Route3_EventScript_PatrouilleRecon::
	trainerbattle_single TRAINER_PATROUILLE_RECON_HNS, HeartSoul_Text_PatrouilleReconSeen, HeartSoul_Text_PatrouilleReconBeaten, Route3_EventScript_PatrouilleReconVictoire
	msgbox HeartSoul_Text_PatrouilleReconAfterBattle, MSGBOX_AUTOCLOSE
	end

Route3_EventScript_PatrouilleReconVictoire::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox HeartSoul_Text_PatrouilleReconAfterBattle, MSGBOX_DEFAULT
	setflag FLAG_EMBUSCADE_RECON_VAINCUE
	closemessage
	release
	end

HeartSoul_Text_PatrouilleReconSeen:
	.string "GRUNT: Une patrouille de\n"
	.string "reconnaissance. On teste juste\l"
	.string "le terrain avant les vraies\l"
	.string "troupes.$"

HeartSoul_Text_PatrouilleReconBeaten:
	.string "GRUNT: …\p"
	.string "Ils vont vouloir un rapport\n"
	.string "plus complet que ca.$"

HeartSoul_Text_PatrouilleReconAfterBattle:
	.string "Le GRUNT bat en retraite en\n"
	.string "direction du nord.$"

// Evenement 13 (Acte IV, Celadopole) : dresseur convaincu par le discours de BLUE mais pas
// violent, sa defaite ebranle ses certitudes sans le convertir. TRAINER_SUBORDONNE_
// ADMIRATIF_HNS deja alloue. Position verifiee par decodage collision.
CeladonCity_EventScript_SubordonneAdmiratif::
	trainerbattle_single TRAINER_SUBORDONNE_ADMIRATIF_HNS, HeartSoul_Text_SubordonneAdmiratifSeen, HeartSoul_Text_SubordonneAdmiratifBeaten, CeladonCity_EventScript_SubordonneAdmiratifVictoire
	msgbox HeartSoul_Text_SubordonneAdmiratifAfterBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_EventScript_SubordonneAdmiratifVictoire::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox HeartSoul_Text_SubordonneAdmiratifAfterBattle, MSGBOX_DEFAULT
	setflag FLAG_SUBORDONNE_ADMIRATIF_VAINCU
	closemessage
	release
	end

HeartSoul_Text_SubordonneAdmiratifSeen:
	.string "SUBORDONNE: BLUE a raison sur\n"
	.string "un point : la Ligue ne\l"
	.string "recompense pas le talent. Je\l"
	.string "vais te le prouver.$"

HeartSoul_Text_SubordonneAdmiratifBeaten:
	.string "SUBORDONNE: …\p"
	.string "Ce n'etait pas cense se passer\n"
	.string "comme ca.$"

HeartSoul_Text_SubordonneAdmiratifAfterBattle:
	.string "SUBORDONNE: …Peut-etre que la\n"
	.string "force ne suffit pas non plus.\l"
	.string "Peut-etre.$"
