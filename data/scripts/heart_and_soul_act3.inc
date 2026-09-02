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
	closemessage
	release
	end

HeartSoul_Text_LyreSeen:
	.string "LYRE: Encore une dresseuse qui\n"
	.string "vient jouer les héroïnes dans MA\l"
	.string "forêt.\p"
	.string "La TEAM ROCKET va rendre ces\n"
	.string "bois aux POKéMON. Toi, tu ne\l"
	.string "fais que passer.$"

HeartSoul_Text_LyreBeaten:
	.string "LYRE: …\p"
	.string "Peut-être que la force ne suffit\n"
	.string "pas, en fin de compte.$"

HeartSoul_Text_LyreAfterBattle:
	.string "LYRE: Je reste convaincue que\n"
	.string "cette forêt méritait d'être\l"
	.string "protégée. Sur les moyens, par\l"
	.string "contre… je commence à douter.$"

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
	.string "pas lu le bon.$"
