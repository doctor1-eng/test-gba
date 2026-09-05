// Heart & Soul - histoire.md section 12 : 8 "aspirants Champions" en rencontre aleatoire, un
// par type non encore represente officiellement (Vol/Insecte/Combat/Glace/Dragon/Tenebres/
// Acier/Fee). Contenu optionnel, independant de la progression scenaristique.
//
// Simplifications assumees et documentees (voir aussi le commentaire au-dessus de
// FLAG_HIDE_AEL dans flags_hns.h) :
// - Niveau fixe 34 (comme tout le reste de Heart & Soul) plutot que cale dynamiquement sur le
//   niveau moyen de l'equipe du joueur : aucune fonction n'est exposee au niveau script pour
//   lire ce niveau moyen et l'appliquer a un trainerbattle_single classique dans ce fork
//   (GetHighestLevelInPlayerParty existe cote C mais uniquement pour la Battle Frontier) -
//   implementer une vraie mise a l'echelle dynamique demanderait du nouveau code moteur, hors
//   perimetre de ce chantier.
// - Pas de "cooldown hebdomadaire" ni d'equipe qui se renforce a chaque revanche : chaque
//   aspirant reste rejouable a chaque nouvelle apparition aleatoire, avec la meme equipe.
//   Contenu optionnel/rejouable prevu, boucle de progression VS-Seeker non implementee.
// - Corvin ("n'importe quelle route, uniquement de nuit") place sur une seule route
//   (Route 1) plutot que sur "n'importe quelle route" au sens litteral - toutes les routes
//   n'ont pas ete instrumentees, seule Route 1 le check + la contrainte de nuit (gettimeofday,
//   meme mecanisme deja utilise ailleurs dans ce fork, ex. AkalaIsle_hns).
//
// Mecanique de rencontre : chaque route/ville concernee a deja un MAP_SCRIPT_ON_TRANSITION
// existant (ou en obtient un) qui appelle le sous-script HeartSoul_EventScript_Check<Nom>
// ci-dessous en premiere ligne (avant toute autre logique de la map, pour s'executer a
// chaque entree independamment des autres branches) : un jet `random 20` (1 chance sur 20)
// determine si l'aspirant est visible (clearflag FLAG_HIDE_<NOM>) ou non (setflag) pour cette
// visite. object_type TRAINER_TYPE_NORMAL (approche automatique), meme patron que Quinn/Lyre/
// Selen deja valide (trainerbattle_single avec event_script de victoire, flag pose
// uniquement a la victoire).

HeartSoul_EventScript_CheckAel::
	random 20
	goto_if_ne VAR_RESULT, 0, HeartSoul_CheckAel_Hide
	clearflag FLAG_HIDE_AEL
	return
HeartSoul_CheckAel_Hide:
	setflag FLAG_HIDE_AEL
	return

HeartSoul_EventScript_CheckNao::
	random 20
	goto_if_ne VAR_RESULT, 0, HeartSoul_CheckNao_Hide
	clearflag FLAG_HIDE_NAO
	return
HeartSoul_CheckNao_Hide:
	setflag FLAG_HIDE_NAO
	return

HeartSoul_EventScript_CheckRook::
	random 20
	goto_if_ne VAR_RESULT, 0, HeartSoul_CheckRook_Hide
	clearflag FLAG_HIDE_ROOK
	return
HeartSoul_CheckRook_Hide:
	setflag FLAG_HIDE_ROOK
	return

HeartSoul_EventScript_CheckIris::
	random 20
	goto_if_ne VAR_RESULT, 0, HeartSoul_CheckIris_Hide
	clearflag FLAG_HIDE_IRIS_ASPIRANTE
	return
HeartSoul_CheckIris_Hide:
	setflag FLAG_HIDE_IRIS_ASPIRANTE
	return

HeartSoul_EventScript_CheckOrin::
	random 20
	goto_if_ne VAR_RESULT, 0, HeartSoul_CheckOrin_Hide
	clearflag FLAG_HIDE_ORIN
	return
HeartSoul_CheckOrin_Hide:
	setflag FLAG_HIDE_ORIN
	return

HeartSoul_EventScript_CheckFerra::
	random 20
	goto_if_ne VAR_RESULT, 0, HeartSoul_CheckFerra_Hide
	clearflag FLAG_HIDE_FERRA
	return
HeartSoul_CheckFerra_Hide:
	setflag FLAG_HIDE_FERRA
	return

HeartSoul_EventScript_CheckLila::
	random 20
	goto_if_ne VAR_RESULT, 0, HeartSoul_CheckLila_Hide
	clearflag FLAG_HIDE_LILA_ASPIRANTE
	return
HeartSoul_CheckLila_Hide:
	setflag FLAG_HIDE_LILA_ASPIRANTE
	return

// Corvin : meme jet aleatoire, mais uniquement si gettimeofday renvoie TIME_NIGHT.
HeartSoul_EventScript_CheckCorvin::
	gettimeofday
	compare VAR_0x8000, TIME_NIGHT
	goto_if_ne HeartSoul_CheckCorvin_Hide
	random 20
	goto_if_ne VAR_RESULT, 0, HeartSoul_CheckCorvin_Hide
	clearflag FLAG_HIDE_CORVIN
	return
HeartSoul_CheckCorvin_Hide:
	setflag FLAG_HIDE_CORVIN
	return

// --- Ael (Vol, Route 21) ---
Route21_EventScript_Ael::
	trainerbattle_single TRAINER_AEL_HNS, HeartSoul_Text_AelSeen, HeartSoul_Text_AelBeaten, Route21_EventScript_AelVictoire
	msgbox HeartSoul_Text_AelAfterBattle, MSGBOX_AUTOCLOSE
	end

Route21_EventScript_AelVictoire::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox HeartSoul_Text_AelAfterBattle, MSGBOX_DEFAULT
	goto_if_set FLAG_AEL_RENCONTRE, HeartSoul_AelVictoire_DejaCompte
	setflag FLAG_AEL_RENCONTRE
	addvar VAR_ASPIRANTS_BATTUS, 1
HeartSoul_AelVictoire_DejaCompte:
	closemessage
	release
	end

HeartSoul_Text_AelSeen:
	.string "AEL: Ces POKéMON Vol\n"
	.string "meritent d'être observés, pas\l"
	.string "juste capturés. Montre-moi que\l"
	.string "tu les respectes autant que\l"
	.string "moi.$"

HeartSoul_Text_AelBeaten:
	.string "AEL: …\p"
	.string "Bien joué. Vraiment.$"

HeartSoul_Text_AelAfterBattle:
	.string "AEL: Un jour, je serai\n"
	.string "Championne de type Vol. Ce\l"
	.string "jour-la, reviens me voir.$"

// --- Nao (Insecte, Foret de Jade) ---
ViridianForest_EventScript_Nao::
	trainerbattle_single TRAINER_NAO_HNS, HeartSoul_Text_NaoSeen, HeartSoul_Text_NaoBeaten, ViridianForest_EventScript_NaoVictoire
	msgbox HeartSoul_Text_NaoAfterBattle, MSGBOX_AUTOCLOSE
	end

ViridianForest_EventScript_NaoVictoire::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox HeartSoul_Text_NaoAfterBattle, MSGBOX_DEFAULT
	goto_if_set FLAG_NAO_RENCONTRE, HeartSoul_NaoVictoire_DejaCompte
	setflag FLAG_NAO_RENCONTRE
	addvar VAR_ASPIRANTS_BATTUS, 1
HeartSoul_NaoVictoire_DejaCompte:
	closemessage
	release
	end

HeartSoul_Text_NaoSeen:
	.string "NAO: Euh… d-désolé, je\n"
	.string "collectionnais des specimens,\l"
	.string "mais… un combat, why not ?\p"
	.string "LYRE avait raison de défendre\n"
	.string "cette forêt, tu sais.$"

HeartSoul_Text_NaoBeaten:
	.string "NAO: Ah… c'était fascinant à\n"
	.string "observer, au moins.$"

HeartSoul_Text_NaoAfterBattle:
	.string "NAO: Les Insecte méritent leur\n"
	.string "propre ARENE. J'y travaille.$"

// --- Rook (Combat, Route 15) ---
Route15_EventScript_Rook::
	trainerbattle_single TRAINER_ROOK_HNS, HeartSoul_Text_RookSeen, HeartSoul_Text_RookBeaten, Route15_EventScript_RookVictoire
	msgbox HeartSoul_Text_RookAfterBattle, MSGBOX_AUTOCLOSE
	end

Route15_EventScript_RookVictoire::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox HeartSoul_Text_RookAfterBattle, MSGBOX_DEFAULT
	goto_if_set FLAG_ROOK_RENCONTRE, HeartSoul_RookVictoire_DejaCompte
	setflag FLAG_ROOK_RENCONTRE
	addvar VAR_ASPIRANTS_BATTUS, 1
HeartSoul_RookVictoire_DejaCompte:
	closemessage
	release
	end

HeartSoul_Text_RookSeen:
	.string "ROOK: La puissance brute sans\n"
	.string "technique n'est que du bruit.\l"
	.string "Le DOJO de Fuchsia m'a appris\l"
	.string "mieux que ça.$"

HeartSoul_Text_RookBeaten:
	.string "ROOK: …\p"
	.string "Ta technique parle pour toi.$"

HeartSoul_Text_RookAfterBattle:
	.string "ROOK: Continue comme ça, et tu\n"
	.string "seras plus dure à battre que\l"
	.string "moi un jour.$"

// --- Iris (Glace, Seafoam Islands) ---
SeafoamIslands_1F_EventScript_Iris::
	trainerbattle_single TRAINER_IRIS_ASPIRANTE_HNS, HeartSoul_Text_IrisAspiranteSeen, HeartSoul_Text_IrisAspiranteBeaten, SeafoamIslands_1F_EventScript_IrisVictoire
	msgbox HeartSoul_Text_IrisAspiranteAfterBattle, MSGBOX_AUTOCLOSE
	end

SeafoamIslands_1F_EventScript_IrisVictoire::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox HeartSoul_Text_IrisAspiranteAfterBattle, MSGBOX_DEFAULT
	goto_if_set FLAG_IRIS_ASPIRANTE_RENCONTRE, HeartSoul_IrisAspiranteVictoire_DejaCompte
	setflag FLAG_IRIS_ASPIRANTE_RENCONTRE
	addvar VAR_ASPIRANTS_BATTUS, 1
HeartSoul_IrisAspiranteVictoire_DejaCompte:
	closemessage
	release
	end

HeartSoul_Text_IrisAspiranteSeen:
	.string "IRIS: …\p"
	.string "Bats-toi. Ou pars.$"

HeartSoul_Text_IrisAspiranteBeaten:
	.string "IRIS: …\p"
	.string "Suffisant.$"

HeartSoul_Text_IrisAspiranteAfterBattle:
	.string "IRIS: …\p"
	.string "(Elle est déjà repartie.)$"

// --- Orin (Dragon, Route 21) ---
Route21_EventScript_Orin::
	trainerbattle_single TRAINER_ORIN_HNS, HeartSoul_Text_OrinSeen, HeartSoul_Text_OrinBeaten, Route21_EventScript_OrinVictoire
	msgbox HeartSoul_Text_OrinAfterBattle, MSGBOX_AUTOCLOSE
	end

Route21_EventScript_OrinVictoire::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox HeartSoul_Text_OrinAfterBattle, MSGBOX_DEFAULT
	goto_if_set FLAG_ORIN_RENCONTRE, HeartSoul_OrinVictoire_DejaCompte
	setflag FLAG_ORIN_RENCONTRE
	addvar VAR_ASPIRANTS_BATTUS, 1
HeartSoul_OrinVictoire_DejaCompte:
	closemessage
	release
	end

HeartSoul_Text_OrinSeen:
	.string "ORIN: Les Dragon sont les\n"
	.string "derniers vrais mystères de ce\l"
	.string "monde ! Prouve que tu es digne\l"
	.string "d'en approcher un !$"

HeartSoul_Text_OrinBeaten:
	.string "ORIN: …\p"
	.string "Magnifique ! Absolument\n"
	.string "magnifique !$"

HeartSoul_Text_OrinAfterBattle:
	.string "ORIN: Un jour, KANTO aura son\n"
	.string "ARENE Dragon. J'y crois dur\l"
	.string "comme fer !$"

// --- Corvin (Tenebres, Route 1, uniquement de nuit) ---
Route1_EventScript_Corvin::
	trainerbattle_single TRAINER_CORVIN_HNS, HeartSoul_Text_CorvinSeen, HeartSoul_Text_CorvinBeaten, Route1_EventScript_CorvinVictoire
	msgbox HeartSoul_Text_CorvinAfterBattle, MSGBOX_AUTOCLOSE
	end

Route1_EventScript_CorvinVictoire::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox HeartSoul_Text_CorvinAfterBattle, MSGBOX_DEFAULT
	goto_if_set FLAG_CORVIN_RENCONTRE, HeartSoul_CorvinVictoire_DejaCompte
	setflag FLAG_CORVIN_RENCONTRE
	addvar VAR_ASPIRANTS_BATTUS, 1
HeartSoul_CorvinVictoire_DejaCompte:
	closemessage
	release
	end

HeartSoul_Text_CorvinSeen:
	.string "CORVIN: La nuit ne ment pas.\n"
	.string "Le jour, si.\p"
	.string "Voyons ce que tu vaux quand\n"
	.string "personne ne regarde.$"

HeartSoul_Text_CorvinBeaten:
	.string "CORVIN: …\p"
	.string "Pas mal, pour quelqu'un du\n"
	.string "jour.$"

HeartSoul_Text_CorvinAfterBattle:
	.string "CORVIN: On se recroisera. La\n"
	.string "nuit revient toujours.$"

// --- Ferra (Acier, Route de la Centrale) ---
RockTunnel_1F_EventScript_Ferra::
	trainerbattle_single TRAINER_FERRA_HNS, HeartSoul_Text_FerraSeen, HeartSoul_Text_FerraBeaten, RockTunnel_1F_EventScript_FerraVictoire
	msgbox HeartSoul_Text_FerraAfterBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_FerraVictoire::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox HeartSoul_Text_FerraAfterBattle, MSGBOX_DEFAULT
	goto_if_set FLAG_FERRA_RENCONTRE, HeartSoul_FerraVictoire_DejaCompte
	setflag FLAG_FERRA_RENCONTRE
	addvar VAR_ASPIRANTS_BATTUS, 1
HeartSoul_FerraVictoire_DejaCompte:
	closemessage
	release
	end

HeartSoul_Text_FerraSeen:
	.string "FERRA: Mes POKéMON sont des\n"
	.string "machines de précision. Voyons\l"
	.string "si les tiens tiennent la\l"
	.string "cadence.$"

HeartSoul_Text_FerraBeaten:
	.string "FERRA: …\p"
	.string "Bien calibré.$"

HeartSoul_Text_FerraAfterBattle:
	.string "FERRA: Un jour, l'ARENE Acier\n"
	.string "existera. J'y veillerai\l"
	.string "personnellement.$"

// --- Lila (Fee, Celadopole) ---
CeladonCity_EventScript_Lila::
	trainerbattle_single TRAINER_LILA_ASPIRANTE_HNS, HeartSoul_Text_LilaAspiranteSeen, HeartSoul_Text_LilaAspiranteBeaten, CeladonCity_EventScript_LilaVictoire
	msgbox HeartSoul_Text_LilaAspiranteAfterBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_EventScript_LilaVictoire::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox HeartSoul_Text_LilaAspiranteAfterBattle, MSGBOX_DEFAULT
	goto_if_set FLAG_LILA_ASPIRANTE_RENCONTRE, HeartSoul_LilaAspiranteVictoire_DejaCompte
	setflag FLAG_LILA_ASPIRANTE_RENCONTRE
	addvar VAR_ASPIRANTS_BATTUS, 1
HeartSoul_LilaAspiranteVictoire_DejaCompte:
	closemessage
	release
	end

HeartSoul_Text_LilaAspiranteSeen:
	.string "LILA: J'ai quitté mon poste au\n"
	.string "grand magasin après le\l"
	.string "scandale. Depuis, je ne\l"
	.string "m'entraîne que pour ça.$"

HeartSoul_Text_LilaAspiranteBeaten:
	.string "LILA: …\p"
	.string "Charmant. Vraiment charmant.$"

HeartSoul_Text_LilaAspiranteAfterBattle:
	.string "LILA: L'ARENE Fee viendra.\n"
	.string "En attendant, je m'entraîne.$"
