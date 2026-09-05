// Heart & Soul - Acte III : l'ampleur du reseau (docs/heart_and_soul/docs/histoire.md
// section 3). Premier lieutenant scripte : Lyre (Foret de Jade / ViridianForest_hns).
//
// Lyre est un nouvel objet-evenement (pas un NPC existant reflavore : aucun trainer
// generique disponible ne correspondait a "lieutenant regional de la TEAM ROCKET" dans
// cette zone - voir docs/heart_and_soul/technical_map.md). Sprite OBJ_EVENT_GFX_ROCKET_F_HNS
// (deja present dans le jeu de base, jamais utilise pour un PNJ nomme dans ce fork),
// positionne en (46,40) sur ViridianForest_hns : verifie par decodage direct de
// data/layouts/ViridianForest_hns/map.bin (collision 0 sur elle-meme et les 8 tuiles
// voisines, aucun autre object_event/warp sur cette case), meme methode que l'audit des
// portes de Cinnabar.
//
// Choix deliberement aligne sur la convention deja en place pour Quinn/Doug/Ed
// (data/maps/Route1_hns et Route2_hns/scripts.inc) : le combat lui-meme (trainerbattle_single,
// TRAINER_TYPE_NORMAL, approche automatique) n'est PAS conditionne par un flag d'acte -
// aucun dresseur de ce fork ne l'est, seul le contenu narratif qui suit un combat l'est.
// Lyre reste donc combattable des que le joueur atteint la Foret de Jade, y compris avant la
// fin officielle de l'Acte II ; c'est une simplification assumee (monde ouvert, niveaux a
// plat 34 partout), pas un oubli - a documenter si une regle de gating stricte est demandee
// plus tard.
ViridianForest_EventScript_Lyre::
	trainerbattle_single TRAINER_LYRE_HNS, HeartSoul_Text_LyreSeen, HeartSoul_Text_LyreBeaten, ViridianForest_EventScript_LyrePostBattle
	msgbox HeartSoul_Text_LyreAfterBattle, MSGBOX_AUTOCLOSE
	end

ViridianForest_EventScript_LyrePostBattle::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox HeartSoul_Text_LyreAfterBattle, MSGBOX_DEFAULT
	setflag FLAG_LYRE_VAINCUE
	setflag FLAG_CHAMPION_ALLIE_SAUVE
	closemessage
	release
	end

HeartSoul_Text_LyreSeen:
	.string "LYRE: Encore une dresseuse qui\n"
	.string "vient jouer les héroïnes dans MA\l"
	.string "forêt.\p"
	.string "La TEAM ROCKET va rendre ces\n"
	.string "bois aux POKÉMON. Toi, tu ne\l"
	.string "fais que passer.$"

HeartSoul_Text_LyreBeaten:
	.string "LYRE: …\p"
	.string "Peut-être que la force ne suffit\n"
	.string "pas, en fin de compte.$"

HeartSoul_Text_LyreAfterBattle:
	.string "LYRE: Je reste convaincue que\n"
	.string "cette forêt méritait d'être\l"
	.string "protégée. Sur les moyens, par\l"
	.string "contre… je commence à douter.\p"
	.string "…Attends. Une info vient de\n"
	.string "circuler : PIERRE, capturé près\l"
	.string "d'ici il y a quelques heures.\p"
	.string "Mes hommes ne le savaient pas\n"
	.string "encore. Je le fais relâcher tout\l"
	.string "de suite, sans condition.$"

// Deuxieme lieutenante scriptee : Selen (Mont Selenite / MtMoon_Outside_hns). Meme methode
// que Lyre : nouvel objet-evenement (aucun PNJ existant a reflavorer sur cette carte -
// contrairement a ViridianForest_hns, MtMoon_Outside_hns n'a aucun dresseur, seulement des
// Pokemon sauvages Clefairy/Jirachi et une pierre Lune), meme sprite
// OBJ_EVENT_GFX_ROCKET_F_HNS (coherence visuelle entre lieutenants), positionne en (13,10) -
// au centre du groupe de Clefairy deja place sur cette carte (coherent avec sa fiche
// "mystique obsedee par... le folklore Clefairy"), verifie par decodage de
// data/layouts/MtMoon_Outside_hns/map.bin (collision 0 sur la case et son rayon d'erreance
// complet 2x2, aucun chevauchement avec un object_event/warp existant).
//
// Meme choix que Lyre : combat non conditionne par un flag d'acte (aucun dresseur de ce fork
// ne l'est).
MtMoon_Outside_EventScript_Selen::
	trainerbattle_single TRAINER_SELEN_HNS, HeartSoul_Text_SelenSeen, HeartSoul_Text_SelenBeaten, MtMoon_Outside_EventScript_SelenPostBattle
	msgbox HeartSoul_Text_SelenAfterBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_Outside_EventScript_SelenPostBattle::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox HeartSoul_Text_SelenAfterBattle, MSGBOX_DEFAULT
	setflag FLAG_SELEN_VAINCUE
	closemessage
	release
	end

HeartSoul_Text_SelenSeen:
	.string "SELEN: …Les météorites ne\n"
	.string "tombent jamais par hasard.\p"
	.string "BLUE en est la preuve vivante :\n"
	.string "un élu, venu révéler ce que\l"
	.string "KANTO refuse de voir.\p"
	.string "Toi aussi, tu vas comprendre.$"

HeartSoul_Text_SelenBeaten:
	.string "SELEN: …\p"
	.string "Non. Ce n'était pas écrit\n"
	.string "comme ça.$"

HeartSoul_Text_SelenAfterBattle:
	.string "SELEN: Un signe peut en cacher\n"
	.string "un autre. Peut-être que je n'ai\l"
	.string "pas lu le bon.\p"
	.string "Les CLEFAIRY dansent encore\n"
	.string "certaines nuits, autour des\l"
	.string "cratères. Ça, au moins, ce\l"
	.string "n'est pas une légende.$"

// Troisieme lieutenant scripte : Terrence (Route de la Centrale / RockTunnel_1F_hns), seul
// des trois a porter la mecanique de conviction a 3 choix (histoire.md section 3
// "Mecanique de conviction (Terrence & Kess)"). Meme constat que Selen : aucun PNJ dresseur
// existant sur RockTunnel_1F_hns (uniquement item balls et rochers casssables), donc nouvel
// object_event, sprite OBJ_EVENT_GFX_ROCKET_M_HNS (variante masculine du meme choix visuel
// que Lyre/Selen). Position (17,20) verifiee par decodage de
// data/layouts/RockTunnel_1F_hns/map.bin, rayon de securite 5x5 (marge superieure aux deux
// precedents car ce PNJ est immobile - MOVEMENT_TYPE_FACE_DOWN, pas d'erreance : coherent
// avec un mercenaire qui "garde" un point precis plutot qu'une embuscade mobile).
//
// A la difference de Lyre/Selen (TRAINER_TYPE_NORMAL, approche automatique), Terrence est
// TRAINER_TYPE_NONE : le joueur doit l'aborder (interaction A), plus coherent avec une
// negociation qu'une embuscade, et necessaire techniquement puisque la sequence de questions
// doit se derouler AVANT tout declenchement de combat.
//
// Bareme des points (histoire.md) : chaque reponse vaut -1/0/+2 et le total sur 3 questions
// determine l'issue (>=5 convaincu, 3-4 combat allege, <=2 combat complet). Les macros de
// script ne geren pas les entiers negatifs de maniere sure sur VAR_PERSUASION_TERRENCE (var
// 16 bits non signee : un total negatif via subvar boucherait a ~65535 et ferait rater tous
// les seuils goto_if_ge). Solution : decalage de +1 par question (les valeurs -1/0/+2
// deviennent 0/1/3), qui ne change JAMAIS le classement relatif des reponses ni les paliers -
// seuils recalcules en consequence : >=5 devient >=8, 3-4 devient 6-7, <=2 devient <=5 (le
// score decale minimum possible est 1, jamais 0 hors reinitialisation). VAR_PERSUASION_TERRENCE
// vaut exactement 0 uniquement avant la premiere question jamais posee : sert aussi de garde
// pour ne pas reposer les questions a chaque nouvelle visite (une fois calcule, une visite
// ulterieure saute direct a l'issue deja determinee, meme logique que le "flag pose seulement
// a la victoire" deja utilise pour Pierre/Ondine/Lyre/Selen).
//
// Choix C (agressif) de la question 1 court-circuite vers le combat complet SANS poser les
// questions 2/3 (regle explicite de histoire.md, "pour eviter un dialogue qui sonnerait faux
// apres une hostilite affichee") : implemente en fixant directement
// VAR_PERSUASION_TERRENCE a 1 (le score decale minimum, donc <=5, donc combat complet) sans
// passer par Q2/Q3.
RockTunnel_EventScript_Terrence::
	lock
	faceplayer
	goto_if_set FLAG_TERRENCE_CONVAINCU, RockTunnel_Terrence_ApresConvaincu
	goto_if_set FLAG_TERRENCE_COMBAT_ALLEGE, RockTunnel_Terrence_ApresAllege
	goto_if_set FLAG_TERRENCE_VAINCU, RockTunnel_Terrence_ApresVaincu
	goto_if_ne VAR_PERSUASION_TERRENCE, 0, RockTunnel_Terrence_Evaluate
	setflag FLAG_EBOULEMENT_ROCKTUNNEL_VU
	msgbox HeartSoul_Text_EboulementRockTunnel, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_TerrenceIntro, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_TerrenceQ1, MSGBOX_DEFAULT
RockTunnel_Terrence_Q1Choice:
	dynmultipush HeartSoul_Text_TerrenceQ1_A, 0
	dynmultipush HeartSoul_Text_TerrenceQ1_B, 1
	dynmultipush HeartSoul_Text_TerrenceQ1_C, 2
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, RockTunnel_Terrence_Q1_A
	case 1, RockTunnel_Terrence_Q1_B
	case 2, RockTunnel_Terrence_Q1_C
	goto RockTunnel_Terrence_Q1Choice

RockTunnel_Terrence_Q1_A:
	addvar VAR_PERSUASION_TERRENCE, 1
	msgbox HeartSoul_Text_TerrenceQ1_A_Resultat, MSGBOX_DEFAULT
	goto RockTunnel_Terrence_Q2
RockTunnel_Terrence_Q1_B:
	addvar VAR_PERSUASION_TERRENCE, 3
	msgbox HeartSoul_Text_TerrenceQ1_B_Resultat, MSGBOX_DEFAULT
	goto RockTunnel_Terrence_Q2
RockTunnel_Terrence_Q1_C:
	setvar VAR_PERSUASION_TERRENCE, 1
	msgbox HeartSoul_Text_TerrenceQ1_C_Resultat, MSGBOX_DEFAULT
	goto RockTunnel_Terrence_Evaluate

RockTunnel_Terrence_Q2:
	msgbox HeartSoul_Text_TerrenceQ2, MSGBOX_DEFAULT
RockTunnel_Terrence_Q2Choice:
	dynmultipush HeartSoul_Text_TerrenceQ2_A, 0
	dynmultipush HeartSoul_Text_TerrenceQ2_B, 1
	dynmultipush HeartSoul_Text_TerrenceQ2_C, 2
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, RockTunnel_Terrence_Q2_A
	case 1, RockTunnel_Terrence_Q2_B
	case 2, RockTunnel_Terrence_Q2_C
	goto RockTunnel_Terrence_Q2Choice

RockTunnel_Terrence_Q2_A:
	addvar VAR_PERSUASION_TERRENCE, 3
	msgbox HeartSoul_Text_TerrenceQ2_A_Resultat, MSGBOX_DEFAULT
	goto RockTunnel_Terrence_Q3
RockTunnel_Terrence_Q2_B:
	addvar VAR_PERSUASION_TERRENCE, 1
	msgbox HeartSoul_Text_TerrenceQ2_B_Resultat, MSGBOX_DEFAULT
	goto RockTunnel_Terrence_Q3
RockTunnel_Terrence_Q2_C:
	msgbox HeartSoul_Text_TerrenceQ2_C_Resultat, MSGBOX_DEFAULT
	goto RockTunnel_Terrence_Q3

RockTunnel_Terrence_Q3:
	msgbox HeartSoul_Text_TerrenceQ3, MSGBOX_DEFAULT
RockTunnel_Terrence_Q3Choice:
	dynmultipush HeartSoul_Text_TerrenceQ3_A, 0
	dynmultipush HeartSoul_Text_TerrenceQ3_B, 1
	dynmultipush HeartSoul_Text_TerrenceQ3_C, 2
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, RockTunnel_Terrence_Q3_A
	case 1, RockTunnel_Terrence_Q3_B
	case 2, RockTunnel_Terrence_Q3_C
	goto RockTunnel_Terrence_Q3Choice

RockTunnel_Terrence_Q3_A:
	addvar VAR_PERSUASION_TERRENCE, 3
	msgbox HeartSoul_Text_TerrenceQ3_A_Resultat, MSGBOX_DEFAULT
	goto RockTunnel_Terrence_Evaluate
RockTunnel_Terrence_Q3_B:
	msgbox HeartSoul_Text_TerrenceQ3_B_Resultat, MSGBOX_DEFAULT
	goto RockTunnel_Terrence_Evaluate
RockTunnel_Terrence_Q3_C:
	addvar VAR_PERSUASION_TERRENCE, 1
	msgbox HeartSoul_Text_TerrenceQ3_C_Resultat, MSGBOX_DEFAULT
	goto RockTunnel_Terrence_Evaluate

RockTunnel_Terrence_Evaluate:
	goto_if_ge VAR_PERSUASION_TERRENCE, 8, RockTunnel_Terrence_Convaincu
	goto_if_ge VAR_PERSUASION_TERRENCE, 6, RockTunnel_Terrence_CombatAllege
	goto RockTunnel_Terrence_CombatComplet

RockTunnel_Terrence_Convaincu:
	msgbox HeartSoul_Text_TerrenceConvaincu, MSGBOX_DEFAULT
	setflag FLAG_TERRENCE_CONVAINCU
	setflag FLAG_TERRENCE_RESOLU
	addvar VAR_REPUTATION, 2
	closemessage
	release
	end

RockTunnel_Terrence_CombatAllege:
	msgbox HeartSoul_Text_TerrenceCombatAllege, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_TERRENCE_LIGHT_HNS, HeartSoul_Text_TerrenceLightSeen, HeartSoul_Text_TerrenceLightBeaten, RockTunnel_EventScript_TerrenceAllegeVictoire
	msgbox HeartSoul_Text_TerrenceLightAfterBattle, MSGBOX_DEFAULT
	closemessage
	release
	end

RockTunnel_EventScript_TerrenceAllegeVictoire::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox HeartSoul_Text_TerrenceLightAfterBattle, MSGBOX_DEFAULT
	setflag FLAG_TERRENCE_COMBAT_ALLEGE
	setflag FLAG_TERRENCE_RESOLU
	addvar VAR_REPUTATION, 1
	closemessage
	release
	end

RockTunnel_Terrence_CombatComplet:
	msgbox HeartSoul_Text_TerrenceCombatComplet, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_TERRENCE_HNS, HeartSoul_Text_TerrenceSeen, HeartSoul_Text_TerrenceBeaten, RockTunnel_EventScript_TerrenceVictoire
	msgbox HeartSoul_Text_TerrenceAfterBattle, MSGBOX_DEFAULT
	closemessage
	release
	end

RockTunnel_EventScript_TerrenceVictoire::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox HeartSoul_Text_TerrenceAfterBattle, MSGBOX_DEFAULT
	setflag FLAG_TERRENCE_VAINCU
	setflag FLAG_TERRENCE_RESOLU
	closemessage
	release
	end

RockTunnel_Terrence_ApresConvaincu:
	msgbox HeartSoul_Text_TerrenceApresConvaincu, MSGBOX_DEFAULT
	closemessage
	release
	end

RockTunnel_Terrence_ApresAllege:
	msgbox HeartSoul_Text_TerrenceLightAfterBattle, MSGBOX_DEFAULT
	closemessage
	release
	end

RockTunnel_Terrence_ApresVaincu:
	msgbox HeartSoul_Text_TerrenceAfterBattle, MSGBOX_DEFAULT
	closemessage
	release
	end

// Evenement 11 (histoire.md section 11, Acte III) : humanise Terrence avant la sequence de
// conviction, sans changer son bareme de points.
HeartSoul_Text_EboulementRockTunnel:
	.string "Un grondement secoue la\n"
	.string "galerie. Un pan de plafond\l"
	.string "s'effondre entre vous deux.\p"
	.string "Sans un mot, TERRENCE t'aide à\n"
	.string "dégager le passage face à un\l"
	.string "GRAVELER devenu incontrôlable.\p"
	.string "Une fois le calme revenu, il\n"
	.string "reprend son air méfiant.$"

HeartSoul_Text_TerrenceIntro:
	.string "TERRENCE: Encore un pas et je\n"
	.string "te bute. …Ou pas.\p"
	.string "T'as l'air du genre à vouloir\n"
	.string "discuter avant de te battre.\l"
	.string "Vas-y, j'ai deux minutes.$"

HeartSoul_Text_TerrenceQ1:
	.string "TERRENCE: Si tu me bats ici,\n"
	.string "BLUE efface ma dette. C'est ce\l"
	.string "qu'il m'a promis.$"

HeartSoul_Text_TerrenceQ1_A:
	.string "BLUE tient toujours parole$"
HeartSoul_Text_TerrenceQ1_B:
	.string "Tu as déjà vu quelqu'un d'autre\npaye après un échec ?$"
HeartSoul_Text_TerrenceQ1_C:
	.string "Je me fiche de ta dette, bats-toi$"

HeartSoul_Text_TerrenceQ1_A_Resultat:
	.string "TERRENCE: …Ouais. P't-être.$"

HeartSoul_Text_TerrenceQ1_B_Resultat:
	.string "TERRENCE: …\p"
	.string "Non. J'en ai pas vu.$"

HeartSoul_Text_TerrenceQ1_C_Resultat:
	.string "TERRENCE: T'as choisi. On se\n"
	.string "bat, alors.$"

HeartSoul_Text_TerrenceQ2:
	.string "TERRENCE: Et si t'avais une\n"
	.string "meilleure offre à me faire ?$"

HeartSoul_Text_TerrenceQ2_A:
	.string "Un contrat minier légitime,\nvia PIERRE$"
HeartSoul_Text_TerrenceQ2_B:
	.string "Trouve une solution toi-même$"
HeartSoul_Text_TerrenceQ2_C:
	.string "Ce n'est pas mon problème$"

HeartSoul_Text_TerrenceQ2_A_Resultat:
	.string "TERRENCE: …Un vrai contrat ?\n"
	.string "Avec PIERRE ?\p"
	.string "Ça, ça m'intéresse plus que\n"
	.string "les promesses de BLUE.$"

HeartSoul_Text_TerrenceQ2_B_Resultat:
	.string "TERRENCE: Facile à dire, pour\n"
	.string "toi.$"

HeartSoul_Text_TerrenceQ2_C_Resultat:
	.string "TERRENCE: …\p"
	.string "T'es pas venue pour m'aider,\n"
	.string "alors.$"

HeartSoul_Text_TerrenceQ3:
	.string "TERRENCE: Ce tunnel que je\n"
	.string "garde… il saute si BLUE perd.\l"
	.string "Tu le savais ?$"

HeartSoul_Text_TerrenceQ3_A:
	.string "Exactement, et ta ville\nd'origine avec$"
HeartSoul_Text_TerrenceQ3_B:
	.string "Ce n'est qu'un tunnel$"
HeartSoul_Text_TerrenceQ3_C:
	.string "Ne rien dire, passer au combat$"

HeartSoul_Text_TerrenceQ3_A_Resultat:
	.string "TERRENCE: …\p"
	.string "Ma ville. J'y avais même plus\n"
	.string "pense.$"

HeartSoul_Text_TerrenceQ3_B_Resultat:
	.string "TERRENCE: Un tunnel qui m'a\n"
	.string "nourri dix ans. Ouais, c'est\l"
	.string "'qu'un tunnel'.$"

HeartSoul_Text_TerrenceQ3_C_Resultat:
	.string "TERRENCE: …\p"
	.string "Bon. On règle ça autrement,\n"
	.string "alors.$"

HeartSoul_Text_TerrenceConvaincu:
	.string "TERRENCE: …\p"
	.string "D'accord. J'arrête. La dette,\n"
	.string "BLUE peut se la garder.\p"
	.string "Ce tunnel, c'est le mien avant\n"
	.string "d'être le sien.$"

HeartSoul_Text_TerrenceCombatAllege:
	.string "TERRENCE: …T'as pas tout à fait\n"
	.string "tort. Mais ça reste pas gagné\l"
	.string "d'avance.$"

HeartSoul_Text_TerrenceLightSeen:
	.string "TERRENCE: Bon. Montre-moi ce\n"
	.string "que tu vaux, alors.$"

HeartSoul_Text_TerrenceLightBeaten:
	.string "TERRENCE: …\p"
	.string "OK. T'as gagné ça, au moins.$"

HeartSoul_Text_TerrenceLightAfterBattle:
	.string "TERRENCE: J'y réfléchirai. Le\n"
	.string "contrat minier, ça tient\l"
	.string "toujours ?$"

HeartSoul_Text_TerrenceCombatComplet:
	.string "TERRENCE: Ça suffit, les mots.\n"
	.string "J'ai une dette à effacer.$"

HeartSoul_Text_TerrenceSeen:
	.string "TERRENCE: J'ai une dette à\n"
	.string "effacer. Rien de personnel.$"

HeartSoul_Text_TerrenceBeaten:
	.string "TERRENCE: …\p"
	.string "La dette reste. Mais j'ai mal.$"

HeartSoul_Text_TerrenceAfterBattle:
	.string "TERRENCE: T'as gagné. Ça\n"
	.string "n'efface rien, mais… t'as\l"
	.string "gagné.$"

HeartSoul_Text_TerrenceApresConvaincu:
	.string "TERRENCE: Ce tunnel reste sous\n"
	.string "ma garde. La vraie, cette\l"
	.string "fois.$"
