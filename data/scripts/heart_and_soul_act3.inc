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
