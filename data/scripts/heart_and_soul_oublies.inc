// Heart & Soul - "Ceux que la Ligue a oublies" : arc annexe inspire de l'anime, decide et
// construit en autonomie (docs/heart_and_soul/prompts/brief_autonome_ceux_que_la_ligue_a_oublies.md).
//
// Evenement 1 - LE VILLAGE CACHE (anime EP010, "Bulbasaur and the Hidden Village").
// Acces : Route 25, clairiere sous la falaise en (43..51,11) - un BULBIZARRE (object_event
// LOCALID_ROUTE25_BULBIZARRE, (47,11)) s'y tient et file entre les rochers quand on lui parle ;
// une fois le village decouvert, le passage (bg_event (47,10), face nord) permet d'y revenir.
// Carte : MAP_HIDDEN_VILLAGE_HNS (layout genere par tools/hns_maps/hidden_village.py a partir
// des metatiles de Bourg Palette) + interieur MAP_HIDDEN_VILLAGE_HOUSE_HNS.
//
// Pieges connus respectes (CONTEXTE_PROJET section 6) : map_script_2 sur VAR_TEMP_0 ecrit
// a 1 sur tous les chemins (OnTransition le pose des que la scene a deja ete jouee, et la
// scene le pose en premiere instruction) ; aucun enchainement apres un warp ; les
// sous-scripts appeles par call finissent par return.

// ---------------------------------------------------------------------------------------
// Route 25 : le Bulbizarre guide et le passage
// ---------------------------------------------------------------------------------------
Route25_EventScript_BulbizarreGuide::
	lock
	faceplayer
	playmoncry SPECIES_BULBASAUR, CRY_MODE_NORMAL
	msgbox Route25_Text_BulbizarreGuide, MSGBOX_DEFAULT
	waitmoncry
	closemessage
	applymovement LOCALID_ROUTE25_BULBIZARRE, Route25_Movement_BulbizarreFile
	waitmovement 0
	setflag FLAG_HIDE_ROUTE25_BULBIZARRE
	removeobject LOCALID_ROUTE25_BULBIZARRE
	msgbox Route25_Text_BulbizarreDisparu, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route25_EventScript_PassageRefuse
	goto Route25_EventScript_EntrerVillage

Route25_EventScript_Passage::
	lock
	goto_if_unset FLAG_VILLAGE_CACHE_DECOUVERT, Route25_EventScript_PassageInconnu
	msgbox Route25_Text_PassageConnu, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route25_EventScript_PassageRefuse
	goto Route25_EventScript_EntrerVillage

Route25_EventScript_PassageInconnu:
	goto_if_set FLAG_HIDE_ROUTE25_BULBIZARRE, Route25_EventScript_PassageTraces
	msgbox Route25_Text_PassageRochers, MSGBOX_DEFAULT
	release
	end

Route25_EventScript_PassageTraces:
	msgbox Route25_Text_PassageTraces, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route25_EventScript_PassageRefuse
	goto Route25_EventScript_EntrerVillage

Route25_EventScript_PassageRefuse:
	release
	end

Route25_EventScript_EntrerVillage:
	closemessage
	playse SE_EXIT
	warp MAP_HIDDEN_VILLAGE_HNS, 12, 2
	waitstate
	end

Route25_Movement_BulbizarreFile:
	face_up
	jump_in_place_up
	walk_fast_up
	set_invisible
	step_end

Route25_Text_BulbizarreGuide:
	.string "Un BULBIZARRE sauvage t'observe,\n"
	.string "sans la moindre peur.\p"
	.string "Il porte une petite cicatrice sur\n"
	.string "le bulbe… Il se retourne vers les\l"
	.string "rochers, comme pour t'inviter.$"

Route25_Text_BulbizarreDisparu:
	.string "BULBIZARRE s'est glissé dans une\n"
	.string "fente entre les rochers !\p"
	.string "Un étroit sentier s'enfonce\n"
	.string "derrière la falaise.\p"
	.string "Le suivre ?$"

Route25_Text_PassageRochers:
	.string "Des rochers couverts de mousse.\n"
	.string "On dirait qu'un petit POKÉMON\l"
	.string "passe souvent par ici.$"

Route25_Text_PassageTraces:
	.string "Des traces de pattes mènent entre\n"
	.string "les rochers. Suivre le sentier ?$"

Route25_Text_PassageConnu:
	.string "Le sentier du VILLAGE CACHÉ.\n"
	.string "Y retourner ?$"

// ---------------------------------------------------------------------------------------
// Village Cache : scripts de carte
// ---------------------------------------------------------------------------------------
HiddenVillage_OnTransition::
	setflag FLAG_HIDE_VILLAGE_COLLECTEURS
	goto_if_set FLAG_VILLAGE_CACHE_DECOUVERT, HiddenVillage_OnTransition_Apres
	setflag FLAG_HIDE_VILLAGE_MELANIE
	setflag FLAG_HIDE_VILLAGE_BULBIZARRE
	setflag FLAG_HIDE_VILLAGE_CANINOS
	end

HiddenVillage_OnTransition_Apres:
	setvar VAR_TEMP_0, 1
	clearflag FLAG_HIDE_VILLAGE_MELANIE
	call HiddenVillage_ApplyBulbizarreVisibility
	call HiddenVillage_ApplyCaninosVisibility
	end

HiddenVillage_ApplyBulbizarreVisibility:
	setflag FLAG_HIDE_VILLAGE_BULBIZARRE
	goto_if_set FLAG_BULBIZARRE_EMMENE, HiddenVillage_Return
	clearflag FLAG_HIDE_VILLAGE_BULBIZARRE
	return

HiddenVillage_ApplyCaninosVisibility:
	setflag FLAG_HIDE_VILLAGE_CANINOS
	goto_if_unset FLAG_POKEMON_BLESSE_SAUVE, HiddenVillage_Return
	clearflag FLAG_HIDE_VILLAGE_CANINOS
	return

HiddenVillage_Return:
	return

// ---------------------------------------------------------------------------------------
// Village Cache : cinematique d'arrivee (une seule fois)
// ---------------------------------------------------------------------------------------
HiddenVillage_EventScript_Arrivee::
	lockall
	setvar VAR_TEMP_0, 1
	msgbox HiddenVillage_Text_Arrivee, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, HiddenVillage_Movement_PlayerDescend
	waitmovement 0
	// Le gardien surgit
	clearflag FLAG_HIDE_VILLAGE_BULBIZARRE
	addobject LOCALID_VILLAGE_BULBIZARRE
	applymovement LOCALID_VILLAGE_BULBIZARRE, HiddenVillage_Movement_BulbizarreSurgit
	waitmovement 0
	playmoncry SPECIES_BULBASAUR, CRY_MODE_ENCOUNTER
	applymovement LOCALID_PLAYER, HiddenVillage_Movement_Exclamation
	waitmovement 0
	msgbox HiddenVillage_Text_GardienSurgit, MSGBOX_DEFAULT
	waitmoncry
	closemessage
	// Fouet Lianes : la camera tremble, le joueur recule
	playse SE_M_TAIL_WHIP
	setvar VAR_0x8004, 1
	setvar VAR_0x8005, 1
	setvar VAR_0x8006, 6
	setvar VAR_0x8007, 3
	special ShakeCamera
	waitstate
	applymovement LOCALID_PLAYER, HiddenVillage_Movement_PlayerRepousse
	waitmovement 0
	msgbox HiddenVillage_Text_FouetLianes, MSGBOX_DEFAULT
	closemessage
	// Melanie accourt
	clearflag FLAG_HIDE_VILLAGE_MELANIE
	addobject LOCALID_VILLAGE_MELANIE
	msgbox HiddenVillage_Text_MelanieCrie, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_VILLAGE_MELANIE, HiddenVillage_Movement_MelanieAccourt
	applymovement LOCALID_VILLAGE_BULBIZARRE, HiddenVillage_Movement_BulbizarreRecule
	waitmovement LOCALID_VILLAGE_BULBIZARRE
	waitmovement LOCALID_VILLAGE_MELANIE
	applymovement LOCALID_PLAYER, HiddenVillage_Movement_FaceLeft
	waitmovement 0
	msgbox HiddenVillage_Text_MelaniePresentation, MSGBOX_DEFAULT
	call_if_set FLAG_REFUGIES_GUIDES, HiddenVillage_EventScript_EchoGuides
	call_if_set FLAG_REFUGIES_CACHES, HiddenVillage_EventScript_EchoCaches
	msgbox HiddenVillage_Text_MelanieCollecteurs, MSGBOX_DEFAULT
	closemessage
	call_if_set FLAG_POKEMON_BLESSE_SAUVE, HiddenVillage_EventScript_RetrouvaillesCaninos
	// Les collecteurs arrivent par le sentier
	playse SE_PIN
	msgbox HiddenVillage_Text_PasSurLeSentier, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_HG_ENCOUNTER_ROCKET, FALSE
	clearflag FLAG_HIDE_VILLAGE_COLLECTEURS
	addobject LOCALID_VILLAGE_COLLECTEUR1
	addobject LOCALID_VILLAGE_COLLECTEUR2
	applymovement LOCALID_VILLAGE_COLLECTEUR1, HiddenVillage_Movement_CollecteurDescend
	applymovement LOCALID_VILLAGE_COLLECTEUR2, HiddenVillage_Movement_CollecteurDescend
	applymovement LOCALID_PLAYER, HiddenVillage_Movement_FaceUp
	waitmovement LOCALID_VILLAGE_COLLECTEUR1
	waitmovement LOCALID_VILLAGE_COLLECTEUR2
	msgbox HiddenVillage_Text_CollecteurOrdre, MSGBOX_DEFAULT
	applymovement LOCALID_VILLAGE_MELANIE, HiddenVillage_Movement_FaceRight
	waitmovement 0
	msgbox HiddenVillage_Text_MelanieRefuse, MSGBOX_DEFAULT
	msgbox HiddenVillage_Text_CollecteurDefi, MSGBOX_DEFAULT
	closemessage
	trainerbattle_two_trainers TRAINER_COLLECTEUR_ROCKET_1_HNS, HiddenVillage_Text_Collecteur1Battu, TRAINER_COLLECTEUR_ROCKET_2_HNS, HiddenVillage_Text_Collecteur2Battu
	msgbox HiddenVillage_Text_CollecteursFuient, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_VILLAGE_COLLECTEUR1, HiddenVillage_Movement_CollecteurFuit
	applymovement LOCALID_VILLAGE_COLLECTEUR2, HiddenVillage_Movement_CollecteurFuit
	waitmovement LOCALID_VILLAGE_COLLECTEUR1
	waitmovement LOCALID_VILLAGE_COLLECTEUR2
	removeobject LOCALID_VILLAGE_COLLECTEUR1
	removeobject LOCALID_VILLAGE_COLLECTEUR2
	setflag FLAG_HIDE_VILLAGE_COLLECTEURS
	fadedefaultbgm
	setflag FLAG_VILLAGE_CACHE_DEFENDU
	addvar VAR_REPUTATION, 1
	applymovement LOCALID_PLAYER, HiddenVillage_Movement_FaceLeft
	waitmovement 0
	msgbox HiddenVillage_Text_MelanieMerci, MSGBOX_DEFAULT
	closemessage
	// Le choix de BULBIZARRE
	applymovement LOCALID_VILLAGE_BULBIZARRE, HiddenVillage_Movement_BulbizarreApproche
	waitmovement 0
	applymovement LOCALID_PLAYER, HiddenVillage_Movement_FaceDown
	waitmovement 0
	playmoncry SPECIES_BULBASAUR, CRY_MODE_NORMAL
	applymovement LOCALID_VILLAGE_BULBIZARRE, HiddenVillage_Movement_Heart
	waitmovement 0
	waitmoncry
	msgbox HiddenVillage_Text_BulbizarreChoisit, MSGBOX_DEFAULT
HiddenVillage_EventScript_ChoixBulbizarre:
	dynmultipush HiddenVillage_Text_OptionEmmener, 0
	dynmultipush HiddenVillage_Text_OptionLaisser, 1
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, HiddenVillage_EventScript_Emmener
	case 1, HiddenVillage_EventScript_Laisser
	goto HiddenVillage_EventScript_ChoixBulbizarre

HiddenVillage_EventScript_Emmener:
	givemon SPECIES_BULBASAUR, 34, ITEM_EVIOLITE, BALL_FRIEND, NATURE_BOLD, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, 31, 31, 31, 31, 31, 31, MOVE_LEECH_SEED, MOVE_SLEEP_POWDER, MOVE_GIGA_DRAIN, MOVE_SLUDGE_BOMB, SHINY_MODE_NEVER
	copyvar VAR_0x8008, VAR_RESULT
	goto_if_eq VAR_0x8008, MON_CANT_GIVE, HiddenVillage_EventScript_EquipePleine
	playfanfare MUS_HG_RECEIVE_POKEMON
	msgbox HiddenVillage_Text_BulbizarreRejoint, MSGBOX_DEFAULT
	waitfanfare
	call_if_eq VAR_0x8008, MON_GIVEN_TO_PC, HiddenVillage_EventScript_VersPC
	setflag FLAG_BULBIZARRE_EMMENE
	removeobject LOCALID_VILLAGE_BULBIZARRE
	setflag FLAG_HIDE_VILLAGE_BULBIZARRE
	msgbox HiddenVillage_Text_MelanieAuRevoirBulbi, MSGBOX_DEFAULT
	goto HiddenVillage_EventScript_FinScene

HiddenVillage_EventScript_EquipePleine:
	msgbox HiddenVillage_Text_EquipePleine, MSGBOX_DEFAULT
	goto HiddenVillage_EventScript_ChoixBulbizarre

HiddenVillage_EventScript_VersPC:
	msgbox HiddenVillage_Text_BulbizarreVersPC, MSGBOX_DEFAULT
	return

HiddenVillage_EventScript_Laisser:
	msgbox HiddenVillage_Text_MelanieGardien, MSGBOX_DEFAULT
	giveitem ITEM_LEFTOVERS
	msgbox HiddenVillage_Text_MelanieRestes, MSGBOX_DEFAULT
HiddenVillage_EventScript_FinScene:
	setflag FLAG_VILLAGE_CACHE_DECOUVERT
	setflag FLAG_HIDE_ROUTE25_BULBIZARRE
	msgbox HiddenVillage_Text_MelanieFin, MSGBOX_DEFAULT
	closemessage
	// Chacun reprend sa place : sans ca, Melanie/Caninos/Bulbizarre encerclent le joueur
	// en (12,3) jusqu'au prochain chargement de la carte.
	fadescreen FADE_TO_BLACK
	setobjectxy LOCALID_VILLAGE_MELANIE, 8, 9
	turnobject LOCALID_VILLAGE_MELANIE, DIR_SOUTH
	setobjectxy LOCALID_VILLAGE_CANINOS, 17, 9
	turnobject LOCALID_VILLAGE_CANINOS, DIR_WEST
	setobjectxy LOCALID_VILLAGE_BULBIZARRE, 12, 6
	turnobject LOCALID_VILLAGE_BULBIZARRE, DIR_NORTH
	fadescreen FADE_FROM_BLACK
	releaseall
	end

HiddenVillage_EventScript_EchoGuides:
	msgbox HiddenVillage_Text_EchoGuides, MSGBOX_DEFAULT
	return

HiddenVillage_EventScript_EchoCaches:
	msgbox HiddenVillage_Text_EchoCaches, MSGBOX_DEFAULT
	return

HiddenVillage_EventScript_RetrouvaillesCaninos:
	clearflag FLAG_HIDE_VILLAGE_CANINOS
	addobject LOCALID_VILLAGE_CANINOS
	playmoncry SPECIES_GROWLITHE, CRY_MODE_NORMAL
	applymovement LOCALID_VILLAGE_CANINOS, HiddenVillage_Movement_CaninosAccourt
	waitmovement 0
	waitmoncry
	applymovement LOCALID_PLAYER, HiddenVillage_Movement_FaceRight
	waitmovement 0
	applymovement LOCALID_VILLAGE_CANINOS, HiddenVillage_Movement_Heart
	waitmovement 0
	msgbox HiddenVillage_Text_RetrouvaillesCaninos, MSGBOX_DEFAULT
	closemessage
	return

// ---------------------------------------------------------------------------------------
// Village Cache : PNJ et Pokemon apres la scene
// ---------------------------------------------------------------------------------------
HiddenVillage_EventScript_Melanie::
	lock
	faceplayer
	goto_if_set FLAG_BULBIZARRE_EMMENE, HiddenVillage_EventScript_MelanieBulbiParti
	msgbox HiddenVillage_Text_MelanieApresGardien, MSGBOX_DEFAULT
	release
	end
HiddenVillage_EventScript_MelanieBulbiParti:
	msgbox HiddenVillage_Text_MelanieApresBulbi, MSGBOX_DEFAULT
	release
	end

HiddenVillage_EventScript_Bulbizarre::
	lock
	faceplayer
	playmoncry SPECIES_BULBASAUR, CRY_MODE_NORMAL
	msgbox HiddenVillage_Text_BulbizarreGarde, MSGBOX_DEFAULT
	waitmoncry
	release
	end

HiddenVillage_EventScript_Caninos::
	lock
	faceplayer
	playmoncry SPECIES_GROWLITHE, CRY_MODE_NORMAL
	msgbox HiddenVillage_Text_Caninos, MSGBOX_DEFAULT
	waitmoncry
	release
	end

HiddenVillage_EventScript_Mystherbe::
	lock
	faceplayer
	playmoncry SPECIES_ODDISH, CRY_MODE_NORMAL
	msgbox HiddenVillage_Text_Mystherbe, MSGBOX_DEFAULT
	waitmoncry
	release
	end

HiddenVillage_EventScript_Carapuce::
	lock
	faceplayer
	playmoncry SPECIES_SQUIRTLE, CRY_MODE_NORMAL
	msgbox HiddenVillage_Text_Carapuce, MSGBOX_DEFAULT
	waitmoncry
	release
	end

HiddenVillage_EventScript_Roucool::
	lock
	faceplayer
	playmoncry SPECIES_PIDGEY, CRY_MODE_NORMAL
	msgbox HiddenVillage_Text_Roucool, MSGBOX_DEFAULT
	waitmoncry
	release
	end

HiddenVillage_EventScript_Psykokwak::
	lock
	faceplayer
	playmoncry SPECIES_PSYDUCK, CRY_MODE_NORMAL
	msgbox HiddenVillage_Text_Psykokwak, MSGBOX_DEFAULT
	waitmoncry
	release
	end

HiddenVillage_EventScript_Salameche::
	lock
	faceplayer
	playmoncry SPECIES_CHARMANDER, CRY_MODE_NORMAL
	msgbox HiddenVillage_Text_Salameche, MSGBOX_DEFAULT
	waitmoncry
	release
	end

HiddenVillage_EventScript_Panneau::
	msgbox HiddenVillage_Text_Panneau, MSGBOX_SIGN
	end

HiddenVillage_EventScript_Sortie::
	lockall
	setvar VAR_TEMP_1, 1
	playse SE_EXIT
	warp MAP_ROUTE25_HNS, 47, 11
	waitstate
	end

HiddenVillage_House_EventScript_Nidoran::
	lock
	faceplayer
	playmoncry SPECIES_NIDORAN_F, CRY_MODE_NORMAL
	msgbox HiddenVillage_House_Text_Nidoran, MSGBOX_DEFAULT
	waitmoncry
	release
	end

HiddenVillage_House_EventScript_Evoli::
	lock
	faceplayer
	playmoncry SPECIES_EEVEE, CRY_MODE_NORMAL
	msgbox HiddenVillage_House_Text_Evoli, MSGBOX_DEFAULT
	waitmoncry
	release
	end

HiddenVillage_House_EventScript_Carnet::
	lockall
	msgbox HiddenVillage_House_Text_Carnet1, MSGBOX_DEFAULT
	msgbox HiddenVillage_House_Text_Carnet2, MSGBOX_DEFAULT
	goto_if_unset FLAG_VILLAGE_CACHE_DEFENDU, HiddenVillage_House_EventScript_CarnetFin
	msgbox HiddenVillage_House_Text_Carnet3, MSGBOX_DEFAULT
HiddenVillage_House_EventScript_CarnetFin:
	releaseall
	end

// ---------------------------------------------------------------------------------------
// Mouvements
// ---------------------------------------------------------------------------------------
HiddenVillage_Movement_PlayerDescend:
	walk_down
	walk_down
	step_end

HiddenVillage_Movement_BulbizarreSurgit:
	jump_in_place_up
	walk_fast_up
	step_end

HiddenVillage_Movement_BulbizarreRecule:
	face_left
	delay_16
	face_up
	step_end

HiddenVillage_Movement_BulbizarreApproche:
	walk_up
	step_end

HiddenVillage_Movement_Exclamation:
	emote_exclamation_mark
	step_end

HiddenVillage_Movement_Heart:
	emote_heart
	step_end

HiddenVillage_Movement_PlayerRepousse:
	lock_facing_direction
	walk_fast_up
	unlock_facing_direction
	step_end

HiddenVillage_Movement_MelanieAccourt:
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	face_right
	step_end

HiddenVillage_Movement_CaninosAccourt:
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	face_left
	step_end

HiddenVillage_Movement_CollecteurDescend:
	walk_down
	walk_down
	step_end

HiddenVillage_Movement_CollecteurFuit:
	face_up
	walk_fast_up
	walk_fast_up
	set_invisible
	step_end

HiddenVillage_Movement_FaceLeft:
	face_left
	step_end

HiddenVillage_Movement_FaceRight:
	face_right
	step_end

HiddenVillage_Movement_FaceUp:
	face_up
	step_end

HiddenVillage_Movement_FaceDown:
	face_down
	step_end

// ---------------------------------------------------------------------------------------
// Textes
// ---------------------------------------------------------------------------------------
HiddenVillage_Text_Arrivee:
	.string "Le sentier débouche sur une\n"
	.string "clairière qu'aucune carte ne\l"
	.string "mentionne.\p"
	.string "Une cabane, des fleurs, et… des\n"
	.string "dizaines de POKÉMON sans dresseur,\l"
	.string "qui vivent ici en paix.$"

HiddenVillage_Text_GardienSurgit:
	.string "Le BULBIZARRE de tout à l'heure\n"
	.string "te barre la route !\p"
	.string "Ses lianes claquent. Il ne te\n"
	.string "laissera pas approcher.$"

HiddenVillage_Text_FouetLianes:
	.string "FOUET LIANES !\n"
	.string "Tu recules sous l'impact.$"

HiddenVillage_Text_MelanieCrie:
	.string "???: BULBIZARRE ! Arrête !\n"
	.string "Ce n'est pas l'un d'EUX !$"

HiddenVillage_Text_MelaniePresentation:
	.string "MÉLANIE: Pardon. Il protège le\n"
	.string "village de tout ce qui vient de\l"
	.string "l'extérieur. Moi, c'est MÉLANIE.\p"
	.string "Ici, j'accueille les POKÉMON\n"
	.string "que leurs dresseurs ont laissés\l"
	.string "derrière eux. Blessés, perdus,\l"
	.string "ou simplement abandonnés.\p"
	.string "Et depuis la chute de CINNABAR,\n"
	.string "ils arrivent par dizaines.$"

HiddenVillage_Text_EchoGuides:
	.string "MÉLANIE: Le bateau de réfugiés qui\n"
	.string "a accosté à CARMIN… Les gens ont\l"
	.string "eu la vie sauve, mais on leur a\l"
	.string "interdit de garder leurs POKÉMON\l"
	.string "dans les centres d'accueil.\p"
	.string "Quelqu'un les a guidés jusqu'au\n"
	.string "port, parait-il. Sans ça, il n'y\l"
	.string "aurait eu personne à sauver.$"

HiddenVillage_Text_EchoCaches:
	.string "MÉLANIE: Des habitants de CINNABAR\n"
	.string "se cachent encore là-bas. Un\l"
	.string "pêcheur fait passer leurs POKÉMON\l"
	.string "de nuit, un par un, jusqu'ici.\p"
	.string "Ils ne voulaient pas les exposer\n"
	.string "aux rafles. Moi non plus.$"

HiddenVillage_Text_MelanieCollecteurs:
	.string "MÉLANIE: Mais la TEAM ROCKET a\n"
	.string "commencé à les ramasser. Tous\l"
	.string "les POKÉMON “sans dresseur”,\l"
	.string "pour l'armée de BLUE.\p"
	.string "Ils appellent ça une collecte.$"

HiddenVillage_Text_RetrouvaillesCaninos:
	.string "Un CANINOS au pelage encore\n"
	.string "roussi se jette sur toi !\p"
	.string "C'est celui que tu as soigné\n"
	.string "sur le port de CINNABAR.\p"
	.string "MÉLANIE: Il est arrivé avec les\n"
	.string "autres, il y a trois jours. Il\l"
	.string "cherchait quelqu'un. Il t'a\l"
	.string "reconnu{E_ACCORD} tout de suite.$"

HiddenVillage_Text_PasSurLeSentier:
	.string "Des pas pressés résonnent sur\n"
	.string "le sentier…$"

HiddenVillage_Text_CollecteurOrdre:
	.string "COLLECTEUR: Tiens, tiens. Le\n"
	.string "fameux village fantôme.\p"
	.string "Ordre de BLUE : tout POKÉMON\n"
	.string "sans dresseur enregistré est\l"
	.string "réquisitionné. Pour sa sécurité.$"

HiddenVillage_Text_MelanieRefuse:
	.string "MÉLANIE: Ils n'ont PAS de\n"
	.string "dresseur, justement. Personne ne\l"
	.string "les a jamais protégés. Surtout\l"
	.string "pas votre BLUE.$"

HiddenVillage_Text_CollecteurDefi:
	.string "GLANEUSE: Et qui va nous en\n"
	.string "empêcher ? La fille aux fleurs ?\p"
	.string "…Attends. Cette tête… C'est\n"
	.string "{CHAMPION_DEF} de CINNABAR !\p"
	.string "Ça tombe bien. BLUE paiera double\n"
	.string "pour tes POKÉMON à toi.$"

HiddenVillage_Text_Collecteur1Battu:
	.string "COLLECTEUR: Deux contre un, et\n"
	.string "on perd quand même ?!$"

HiddenVillage_Text_Collecteur2Battu:
	.string "GLANEUSE: Tch… Ce n'était pas\n"
	.string "prévu dans la fiche de mission.$"

HiddenVillage_Text_CollecteursFuient:
	.string "COLLECTEUR: On reviendra. Avec\n"
	.string "une vraie équipe, cette fois.\p"
	.string "GLANEUSE: …Ou pas. Personne\n"
	.string "ne sait où est ce village, de\l"
	.string "toute façon. On l'a trouvé par\l"
	.string "hasard. Filons !$"

HiddenVillage_Text_MelanieMerci:
	.string "MÉLANIE: …Merci.\p"
	.string "Les gens disent que la LIGUE a\n"
	.string "abandonné CINNABAR. Mais toi,\l"
	.string "tu es venu{E_ACCORD} jusqu'ici. Pour\l"
	.string "des POKÉMON que personne ne\l"
	.string "réclame.$"

HiddenVillage_Text_BulbizarreChoisit:
	.string "BULBIZARRE s'approche et se\n"
	.string "blottit contre ta jambe.\p"
	.string "MÉLANIE: Ça alors… Il n'a jamais\n"
	.string "suivi personne. Il t'a choisi{E_ACCORD}.\p"
	.string "Il a besoin de voir autre chose\n"
	.string "que cette clairière. Mais il est\l"
	.string "aussi le seul à défendre les\l"
	.string "autres quand je ne suis pas là.\p"
	.string "C'est à toi de décider.$"

HiddenVillage_Text_OptionEmmener:
	.string "L'emmener avec toi$"

HiddenVillage_Text_OptionLaisser:
	.string "Le laisser veiller ici$"

HiddenVillage_Text_BulbizarreRejoint:
	.string "BULBIZARRE a rejoint {PLAYER} !$"

HiddenVillage_Text_BulbizarreVersPC:
	.string "Ton équipe étant complète,\n"
	.string "BULBIZARRE a été envoyé au PC.$"

HiddenVillage_Text_EquipePleine:
	.string "Ton équipe et ta boîte PC sont\n"
	.string "pleines. Libère de la place\l"
	.string "d'abord.$"

HiddenVillage_Text_MelanieAuRevoirBulbi:
	.string "MÉLANIE: Prends soin de lui.\n"
	.string "Il porte un EVIOLITE : il n'a\l"
	.string "jamais voulu évoluer. C'est son\l"
	.string "choix, et c'est sa force.$"

HiddenVillage_Text_MelanieGardien:
	.string "BULBIZARRE retourne se poster au\n"
	.string "milieu du sentier, fier.\p"
	.string "MÉLANIE: Tant qu'il sera là, plus\n"
	.string "personne n'entrera sans qu'on le\l"
	.string "sache. Merci de lui faire\l"
	.string "confiance. Tiens, prends ça.$"

HiddenVillage_Text_MelanieRestes:
	.string "MÉLANIE: Des RESTES, cueillis\n"
	.string "sur nos baies. Ça aidera tes\l"
	.string "POKÉMON à tenir le coup.$"

HiddenVillage_Text_MelanieFin:
	.string "MÉLANIE: Le passage entre les\n"
	.string "rochers de la ROUTE 25 restera\l"
	.string "ouvert pour toi.\p"
	.string "Et si un jour la LIGUE demande\n"
	.string "où étaient ses héros pendant la\l"
	.string "crise… je saurai quoi répondre.$"

HiddenVillage_Text_MelanieApresGardien:
	.string "MÉLANIE: Depuis ton passage, plus\n"
	.string "aucun collecteur n'est revenu.\p"
	.string "BULBIZARRE ne dort plus que d'un\n"
	.string "œil. Les autres, eux, dorment\l"
	.string "enfin.$"

HiddenVillage_Text_MelanieApresBulbi:
	.string "MÉLANIE: Comment va BULBIZARRE ?\n"
	.string "Je suis sûre qu'il te protège\l"
	.string "comme il protégeait le village.\p"
	.string "Les autres se relaient pour\n"
	.string "monter la garde. Ils apprennent.$"

HiddenVillage_Text_BulbizarreGarde:
	.string "BULBIZARRE: Bulba !\p"
	.string "Il surveille le sentier, sérieux\n"
	.string "comme un garde de la LIGUE.$"

HiddenVillage_Text_Caninos:
	.string "CANINOS: Waf ! Waf !\p"
	.string "Il se remet bien. Quand tu iras\n"
	.string "chercher BLAINE, il saura suivre\l"
	.string "la piste de son odeur.$"

HiddenVillage_Text_Mystherbe:
	.string "MYSTHERBE se cache à moitié dans\n"
	.string "les fleurs. Il sursaute encore au\l"
	.string "moindre bruit.$"

HiddenVillage_Text_Carapuce:
	.string "CARAPUCE porte encore des\n"
	.string "lunettes de soleil trouvées on\l"
	.string "ne sait où. Il a l'air fier.$"

HiddenVillage_Text_Roucool:
	.string "ROUCOOL: Rou-rou !\p"
	.string "Une de ses ailes est bandée.\n"
	.string "Il s'entraîne à voler bas.$"

HiddenVillage_Text_Psykokwak:
	.string "PSYKOKWAK se tient la tête.\n"
	.string "Il a l'air d'avoir oublié\l"
	.string "pourquoi il est là. Mais il est\l"
	.string "content d'y être.$"

HiddenVillage_Text_Salameche:
	.string "SALAMÈCHE protège la flamme de\n"
	.string "sa queue de la pluie avec une\l"
	.string "grande feuille. Son dresseur lui\l"
	.string "avait dit de l'attendre…$"

HiddenVillage_Text_Panneau:
	.string "REFUGE DE MÉLANIE\p"
	.string "“Les POKÉMON d'ici ne sont à\n"
	.string "personne. Un jour, ils choisiront\l"
	.string "eux-mêmes leur ami.”$"

HiddenVillage_House_Text_Nidoran:
	.string "NIDORAN♀ dort roulée en boule\n"
	.string "sur un vieux pull.$"

HiddenVillage_House_Text_Evoli:
	.string "ÉVOLI te regarde avec curiosité,\n"
	.string "puis se rendort.$"

HiddenVillage_House_Text_Carnet1:
	.string "Le carnet de MÉLANIE. Chaque page\n"
	.string "porte un nom, une date, une\l"
	.string "provenance.\p"
	.string "“SALAMÈCHE - ROUTE 24 - son\n"
	.string "dresseur n'est jamais revenu.”\p"
	.string "“MYSTHERBE - CARMIN - débarqué\n"
	.string "du bateau de CINNABAR.”$"

HiddenVillage_House_Text_Carnet2:
	.string "“CANINOS - CINNABAR - brûlures\n"
	.string "légères, cherche quelqu'un.”\p"
	.string "La liste continue sur des pages\n"
	.string "et des pages.$"

HiddenVillage_House_Text_Carnet3:
	.string "La dernière entrée, d'une écriture\n"
	.string "plus appuyée :\p"
	.string "“Collecteurs repoussés. Pour une\n"
	.string "fois, quelqu'un est venu.”$"

// =======================================================================================
// Evenement 2 - LES ARENES LIBRES (anime "Dark City" : deux arenes non reconnues par la
// LIGUE qui se battent pour une licence).
// Acces : la porte "toujours verrouillee" de Celadopole (26,17), juste a cote du Game
// Corner - facade de financement Rocket (histoire.md section 8). Le bg_event de porte
// verrouillee (Route7_EventScript_Door) y a ete remplace par un vrai warp.
// Carte : MAP_CELADON_CITY_ARENE_LIBRE_HNS, geometrie de l'arene d'Irisia/Cianwood
// (LAYOUT_CIANWOOD_GYM_HNS, deja compilee et utilisee dans ce build).
// Theme : Blue promet une licence "sans jury ni politique" aux arenes que la LIGUE ignore.
// La joueuse, seule vraie Championne de la piece, rend un jugement qui a des consequences
// (reputation, epilogue, coda des aspirants).
// =======================================================================================
CeladonCity_AreneLibre_OnTransition::
	goto_if_unset FLAG_ARENES_LIBRES_RESOLUES, CeladonCity_AreneLibre_OnTransition_Avant
	setvar VAR_TEMP_0, 1
	setflag FLAG_HIDE_ARENE_LIBRE_VESPER
	goto_if_eq VAR_ARENES_LIBRES_JUGEMENT, 2, CeladonCity_AreneLibre_OnTransition_Partis
	clearflag FLAG_HIDE_ARENE_LIBRE_KAZ
	clearflag FLAG_HIDE_ARENE_LIBRE_YAS
	end
CeladonCity_AreneLibre_OnTransition_Partis:
	setflag FLAG_HIDE_ARENE_LIBRE_KAZ
	setflag FLAG_HIDE_ARENE_LIBRE_YAS
	end
CeladonCity_AreneLibre_OnTransition_Avant:
	clearflag FLAG_HIDE_ARENE_LIBRE_KAZ
	clearflag FLAG_HIDE_ARENE_LIBRE_YAS
	setflag FLAG_HIDE_ARENE_LIBRE_VESPER
	end

CeladonCity_AreneLibre_EventScript_Entree::
	lockall
	setvar VAR_TEMP_0, 1
	setflag FLAG_ARENES_LIBRES_VISITEES
	msgbox CeladonCity_AreneLibre_Text_Entree, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, CeladonCity_AreneLibre_Movement_PlayerAvance
	waitmovement 0
	// La rixe : les deux POKEMON s'entrechoquent
	msgbox CeladonCity_AreneLibre_Text_Rixe1, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_ARENE_MACHOPEUR, CeladonCity_AreneLibre_Movement_ClashDroite
	applymovement LOCALID_ARENE_ELEKTEK, CeladonCity_AreneLibre_Movement_ClashGauche
	waitmovement LOCALID_ARENE_MACHOPEUR
	waitmovement LOCALID_ARENE_ELEKTEK
	playse SE_M_MEGA_KICK
	setvar VAR_0x8004, 0
	setvar VAR_0x8005, 2
	setvar VAR_0x8006, 6
	setvar VAR_0x8007, 3
	special ShakeCamera
	waitstate
	playmoncry SPECIES_MACHAMP, CRY_MODE_ENCOUNTER
	waitmoncry
	msgbox CeladonCity_AreneLibre_Text_Rixe2, MSGBOX_DEFAULT
	closemessage
	// Ils remarquent la joueuse
	applymovement LOCALID_ARENE_KAZ, CeladonCity_AreneLibre_Movement_EmoteFaceDown
	applymovement LOCALID_ARENE_YAS, CeladonCity_AreneLibre_Movement_EmoteFaceDown
	waitmovement LOCALID_ARENE_KAZ
	waitmovement LOCALID_ARENE_YAS
	msgbox CeladonCity_AreneLibre_Text_Reconnue, MSGBOX_DEFAULT
	closemessage
	// Vesper descend des gradins
	playbgm MUS_HG_ENCOUNTER_ROCKET, FALSE
	clearflag FLAG_HIDE_ARENE_LIBRE_VESPER
	addobject LOCALID_ARENE_VESPER
	msgbox CeladonCity_AreneLibre_Text_VesperVoix, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_ARENE_VESPER, CeladonCity_AreneLibre_Movement_VesperDescend
	applymovement LOCALID_PLAYER, CeladonCity_AreneLibre_Movement_FaceUp
	waitmovement LOCALID_ARENE_VESPER
	msgbox CeladonCity_AreneLibre_Text_VesperOffre, MSGBOX_DEFAULT
	applymovement LOCALID_ARENE_KAZ, CeladonCity_AreneLibre_Movement_FaceRight
	applymovement LOCALID_ARENE_YAS, CeladonCity_AreneLibre_Movement_FaceLeft
	waitmovement LOCALID_ARENE_YAS
	msgbox CeladonCity_AreneLibre_Text_KazYasDefi, MSGBOX_DEFAULT
	closemessage
	fadedefaultbgm
	trainerbattle_two_trainers TRAINER_KAZ_HNS, CeladonCity_AreneLibre_Text_KazBattu, TRAINER_YAS_HNS, CeladonCity_AreneLibre_Text_YasBattu
	msgbox CeladonCity_AreneLibre_Text_ApresDuo, MSGBOX_DEFAULT
CeladonCity_AreneLibre_EventScript_Jugement:
	msgbox CeladonCity_AreneLibre_Text_QuestionJugement, MSGBOX_DEFAULT
	dynmultipush CeladonCity_AreneLibre_Text_OptionParrainer, 0
	dynmultipush CeladonCity_AreneLibre_Text_OptionRienDu, 1
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, CeladonCity_AreneLibre_EventScript_Parrainer
	case 1, CeladonCity_AreneLibre_EventScript_RienDu
	goto CeladonCity_AreneLibre_EventScript_Jugement

CeladonCity_AreneLibre_EventScript_Parrainer:
	msgbox CeladonCity_AreneLibre_Text_Parrainage, MSGBOX_DEFAULT
	msgbox CeladonCity_AreneLibre_Text_VesperDefi, MSGBOX_DEFAULT
	closemessage
	trainerbattle_no_intro TRAINER_VESPER_HNS, CeladonCity_AreneLibre_Text_VesperBattue
	// Reputation et issue posees APRES la victoire : une defaite (retour au Centre) rejoue la
	// scene entiere sans double comptage.
	setvar VAR_ARENES_LIBRES_JUGEMENT, 1
	addvar VAR_REPUTATION, 2
	setflag FLAG_VESPER_VAINCUE
	msgbox CeladonCity_AreneLibre_Text_VesperPart, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_ARENE_VESPER, CeladonCity_AreneLibre_Movement_VesperRemonte
	waitmovement 0
	removeobject LOCALID_ARENE_VESPER
	setflag FLAG_HIDE_ARENE_LIBRE_VESPER
	msgbox CeladonCity_AreneLibre_Text_KazYasMerci, MSGBOX_DEFAULT
	giveitem ITEM_EXPERT_BELT
	msgbox CeladonCity_AreneLibre_Text_Cadeau, MSGBOX_DEFAULT
	goto CeladonCity_AreneLibre_EventScript_Fin

CeladonCity_AreneLibre_EventScript_RienDu:
	setvar VAR_ARENES_LIBRES_JUGEMENT, 2
	msgbox CeladonCity_AreneLibre_Text_RienDu, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_ARENE_KAZ, CeladonCity_AreneLibre_Movement_KazRejoint
	applymovement LOCALID_ARENE_YAS, CeladonCity_AreneLibre_Movement_YasRejoint
	waitmovement LOCALID_ARENE_KAZ
	waitmovement LOCALID_ARENE_YAS
	msgbox CeladonCity_AreneLibre_Text_VesperMerci, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_ARENE_VESPER, CeladonCity_AreneLibre_Movement_VesperRemonte
	applymovement LOCALID_ARENE_KAZ, CeladonCity_AreneLibre_Movement_SuitVesper
	applymovement LOCALID_ARENE_YAS, CeladonCity_AreneLibre_Movement_SuitVesper
	waitmovement LOCALID_ARENE_VESPER
	waitmovement LOCALID_ARENE_KAZ
	waitmovement LOCALID_ARENE_YAS
	removeobject LOCALID_ARENE_VESPER
	removeobject LOCALID_ARENE_KAZ
	removeobject LOCALID_ARENE_YAS
	removeobject LOCALID_ARENE_MACHOPEUR
	removeobject LOCALID_ARENE_ELEKTEK
	setflag FLAG_HIDE_ARENE_LIBRE_VESPER
	setflag FLAG_HIDE_ARENE_LIBRE_KAZ
	setflag FLAG_HIDE_ARENE_LIBRE_YAS
	msgbox CeladonCity_AreneLibre_Text_Silence, MSGBOX_DEFAULT
CeladonCity_AreneLibre_EventScript_Fin:
	setflag FLAG_ARENES_LIBRES_RESOLUES
	closemessage
	releaseall
	end

// PNJ apres la scene
CeladonCity_AreneLibre_EventScript_Kaz::
	lock
	faceplayer
	msgbox CeladonCity_AreneLibre_Text_KazApres, MSGBOX_DEFAULT
	release
	end

CeladonCity_AreneLibre_EventScript_Yas::
	lock
	faceplayer
	msgbox CeladonCity_AreneLibre_Text_YasApres, MSGBOX_DEFAULT
	release
	end

CeladonCity_AreneLibre_EventScript_Mackogneur::
	lock
	faceplayer
	playmoncry SPECIES_MACHAMP, CRY_MODE_NORMAL
	msgbox CeladonCity_AreneLibre_Text_Mackogneur, MSGBOX_DEFAULT
	waitmoncry
	release
	end

CeladonCity_AreneLibre_EventScript_Elektek::
	lock
	faceplayer
	playmoncry SPECIES_ELECTABUZZ, CRY_MODE_NORMAL
	msgbox CeladonCity_AreneLibre_Text_Elektek, MSGBOX_DEFAULT
	waitmoncry
	release
	end

CeladonCity_AreneLibre_EventScript_Foule1::
	msgbox CeladonCity_AreneLibre_Text_Foule1, MSGBOX_NPC
	end
CeladonCity_AreneLibre_EventScript_Foule2::
	msgbox CeladonCity_AreneLibre_Text_Foule2, MSGBOX_NPC
	end
CeladonCity_AreneLibre_EventScript_Foule3::
	goto_if_eq VAR_ARENES_LIBRES_JUGEMENT, 2, CeladonCity_AreneLibre_EventScript_Foule3Partis
	msgbox CeladonCity_AreneLibre_Text_Foule3, MSGBOX_NPC
	end
CeladonCity_AreneLibre_EventScript_Foule3Partis:
	msgbox CeladonCity_AreneLibre_Text_Foule3Partis, MSGBOX_NPC
	end
CeladonCity_AreneLibre_EventScript_Foule4::
	msgbox CeladonCity_AreneLibre_Text_Foule4, MSGBOX_NPC
	end
CeladonCity_AreneLibre_EventScript_Foule5::
	msgbox CeladonCity_AreneLibre_Text_Foule5, MSGBOX_NPC
	end
CeladonCity_AreneLibre_EventScript_Statue::
	msgbox CeladonCity_AreneLibre_Text_Statue, MSGBOX_SIGN
	end

CeladonCity_AreneLibre_Movement_PlayerAvance:
	walk_up
	walk_up
	step_end

CeladonCity_AreneLibre_Movement_ClashDroite:
	walk_fast_right
	walk_fast_left
	step_end

CeladonCity_AreneLibre_Movement_ClashGauche:
	walk_fast_left
	walk_fast_right
	step_end

CeladonCity_AreneLibre_Movement_EmoteFaceDown:
	face_down
	emote_exclamation_mark
	step_end

CeladonCity_AreneLibre_Movement_VesperDescend:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

CeladonCity_AreneLibre_Movement_VesperRemonte:
	face_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	set_invisible
	step_end

CeladonCity_AreneLibre_Movement_KazRejoint:
	walk_up
	walk_right
	walk_right
	face_up
	step_end

CeladonCity_AreneLibre_Movement_YasRejoint:
	walk_up
	walk_left
	walk_left
	face_up
	step_end

CeladonCity_AreneLibre_Movement_SuitVesper:
	delay_16
	walk_up
	set_invisible
	step_end

CeladonCity_AreneLibre_Movement_FaceUp:
	face_up
	step_end

CeladonCity_AreneLibre_Movement_FaceRight:
	face_right
	step_end

CeladonCity_AreneLibre_Movement_FaceLeft:
	face_left
	step_end

CeladonCity_AreneLibre_Text_Entree:
	.string "La porte “toujours verrouillée”\n"
	.string "s'ouvre sur un vacarme.\p"
	.string "Des gradins de fortune, une foule\n"
	.string "qui hurle, et au centre… deux\l"
	.string "dresseurs prêts à en découdre.\p"
	.string "Une arène clandestine, à deux pas\n"
	.string "du CASINO de CÉLADOPOLE.$"

CeladonCity_AreneLibre_Text_Rixe1:
	.string "KAZ: Cette licence est pour MON\n"
	.string "arène, YAS ! Dix ans que je forme\l"
	.string "des dresseurs dans un garage !\p"
	.string "YAS: Et moi dans une cave ! Ta\n"
	.string "“arène”, c'est un parking avec\l"
	.string "des tatamis !$"

CeladonCity_AreneLibre_Text_Rixe2:
	.string "FOULE: KAZ ! KAZ ! KAZ !\n"
	.string "FOULE: YAS ! YAS ! YAS !$"

CeladonCity_AreneLibre_Text_Reconnue:
	.string "YAS: …Hé. Attends.\n"
	.string "Cette tête, je la connais.\p"
	.string "KAZ: C'est {CHAMPION_DEF}\n"
	.string "de CINNABAR. En chair et en os.\l"
	.string "Badge officiel, une ville, et un\l"
	.string "siège à la LIGUE.\p"
	.string "YAS: Tout ce qu'on n'aura jamais.$"

CeladonCity_AreneLibre_Text_VesperVoix:
	.string "???: Jamais ? Voilà un mot que\n"
	.string "BLUE a rayé de son vocabulaire.$"

CeladonCity_AreneLibre_Text_VesperOffre:
	.string "VESPER: Je m'appelle VESPER.\n"
	.string "Je recrute pour BLUE.\p"
	.string "Pas de jury, pas d'examen, pas\n"
	.string "de piston. Chez nous, une arène\l"
	.string "qui gagne devient officielle.\p"
	.string "Quand BLUE aura refait la LIGUE,\n"
	.string "la première de ces deux arènes à\l"
	.string "nous rejoindre aura sa licence.\p"
	.string "Et toi, {CHAMPION} ? Tu vas leur\n"
	.string "dire que ton système est juste ?$"

CeladonCity_AreneLibre_Text_KazYasDefi:
	.string "KAZ: Elle a raison sur un point.\n"
	.string "Ton titre, il vaut quoi, au fond ?\p"
	.string "YAS: Pour une fois, on est\n"
	.string "d'accord, KAZ. On va le vérifier.\l"
	.string "Ensemble.\p"
	.string "KAZ: Deux arènes “illégales”\n"
	.string "contre une officielle. Montre-nous\l"
	.string "ce que la LIGUE t'a appris !$"

CeladonCity_AreneLibre_Text_KazBattu:
	.string "KAZ: Tch… Pas volé, ce badge.$"

CeladonCity_AreneLibre_Text_YasBattu:
	.string "YAS: Deux contre un… et rien à\n"
	.string "redire.$"

CeladonCity_AreneLibre_Text_ApresDuo:
	.string "La foule se tait.\p"
	.string "KAZ: …Bon. T'es pas un imposteur.\n"
	.string "Alors dis-le, toi. On vaut quoi ?\p"
	.string "YAS: Est-ce que la LIGUE nous\n"
	.string "doit quelque chose, oui ou non ?$"

CeladonCity_AreneLibre_Text_QuestionJugement:
	.string "Tous les regards sont sur toi.\n"
	.string "Même celui de VESPER.$"

CeladonCity_AreneLibre_Text_OptionParrainer:
	.string "Je vous présente à la LIGUE$"

CeladonCity_AreneLibre_Text_OptionRienDu:
	.string "La LIGUE ne vous doit rien$"

CeladonCity_AreneLibre_Text_Parrainage:
	.string "Tu leur promets de les présenter\n"
	.string "toi-même à l'examen officiel.\l"
	.string "Sans passe-droit. Mais sans porte\l"
	.string "fermée non plus.\p"
	.string "KAZ: …Tu ferais ça ?\n"
	.string "YAS: Devant la LIGUE entière ?$"

CeladonCity_AreneLibre_Text_VesperDefi:
	.string "VESPER: Touchant. Tu leur promets\n"
	.string "ce que la LIGUE a refusé à BLUE.\p"
	.string "Voyons si tes promesses tiennent\n"
	.string "mieux debout que ta ville !$"

CeladonCity_AreneLibre_Text_VesperBattue:
	.string "VESPER: …BLUE m'avait prévenue.\n"
	.string "Tu es de ceux qui y croient\l"
	.string "encore.$"

CeladonCity_AreneLibre_Text_VesperPart:
	.string "VESPER: Garde ta foi en la LIGUE.\n"
	.string "Tu en auras besoin le jour où elle\l"
	.string "te fermera la porte à toi aussi.$"

CeladonCity_AreneLibre_Text_KazYasMerci:
	.string "KAZ: Personne n'avait jamais pris\n"
	.string "notre parti. Personne.\p"
	.string "YAS: On s'entraînera. Ensemble,\n"
	.string "s'il le faut. Et le jour de\l"
	.string "l'examen, tu seras là.\p"
	.string "KAZ: Tiens. C'est tout ce qu'on\n"
	.string "a de précieux, ici.$"

CeladonCity_AreneLibre_Text_Cadeau:
	.string "KAZ: Ma ceinture de maître. Elle\n"
	.string "a traversé plus de matchs que\l"
	.string "n'importe quel badge.$"

CeladonCity_AreneLibre_Text_RienDu:
	.string "Tu leur réponds que la LIGUE ne\n"
	.string "leur doit rien. Qu'ils passent\l"
	.string "leurs badges comme tout le monde.\p"
	.string "KAZ: …Comme tout le monde.\n"
	.string "Évidemment.\p"
	.string "YAS: BLUE avait raison sur un\n"
	.string "point. Vous ne voyez que vos\l"
	.string "badges.$"

CeladonCity_AreneLibre_Text_VesperMerci:
	.string "VESPER: Merci, {CHAMPION}. Tu\n"
	.string "viens de recruter pour nous mieux\l"
	.string "que je ne l'aurais fait.\p"
	.string "Messieurs ? BLUE vous attend.$"

CeladonCity_AreneLibre_Text_Silence:
	.string "Les gradins se vident un à un.\p"
	.string "Il ne reste qu'un tatami usé,\n"
	.string "et le silence.$"

CeladonCity_AreneLibre_Text_KazApres:
	.string "KAZ: YAS et moi, on s'entraîne\n"
	.string "ensemble maintenant. Tu y crois ?\p"
	.string "Le jour de l'examen, je veux que\n"
	.string "la LIGUE voie ce qu'une arène de\l"
	.string "garage peut faire.$"

CeladonCity_AreneLibre_Text_YasApres:
	.string "YAS: Plus de licence en jeu, plus\n"
	.string "de raison de se taper dessus.\p"
	.string "On a monté un vrai programme.\n"
	.string "Les gamins du quartier viennent\l"
	.string "gratuitement.$"

CeladonCity_AreneLibre_Text_Mackogneur:
	.string "MACKOGNEUR fait des pompes sur\n"
	.string "deux bras. Puis sur un. Puis sur\l"
	.string "aucun ?!$"

CeladonCity_AreneLibre_Text_Elektek:
	.string "ÉLEKTEK fait crépiter ses poings.\n"
	.string "Les néons du plafond clignotent.$"

CeladonCity_AreneLibre_Text_Foule1:
	.string "Ici, pas de badge à l'entrée.\n"
	.string "Juste l'envie de se battre.$"

CeladonCity_AreneLibre_Text_Foule2:
	.string "Le CASINO d'à côté ? On dit que\n"
	.string "c'est là que l'argent de la TEAM\l"
	.string "ROCKET se lave. Mais chut.$"

CeladonCity_AreneLibre_Text_Foule3:
	.string "J'ai perdu trois fois contre KAZ.\n"
	.string "La LIGUE ne m'a jamais regardée.\l"
	.string "Lui, il m'a entraînée.$"

CeladonCity_AreneLibre_Text_Foule3Partis:
	.string "Ils sont partis avec la TEAM\n"
	.string "ROCKET. Qui va nous entraîner,\l"
	.string "maintenant ?$"

CeladonCity_AreneLibre_Text_Foule4:
	.string "Selon mes calculs, les arènes\n"
	.string "officielles de KANTO n'ont couvert\l"
	.string "que 8 types sur 18.\p"
	.string "Les 10 autres, ce sont des gens\n"
	.string "comme KAZ et YAS qui les font vivre.$"

CeladonCity_AreneLibre_Text_Foule5:
	.string "Ce sous-sol était une salle de\n"
	.string "jeux du CASINO, avant. Ils l'ont\l"
	.string "fermée du jour au lendemain.$"

CeladonCity_AreneLibre_Text_Statue:
	.string "Une statue de LIGUE… peinte à la\n"
	.string "main. On a rayé “OFFICIELLE” et\l"
	.string "écrit “LIBRE” par-dessus.$"

// =======================================================================================
// Epilogue (appele depuis les 3 paliers de HeartSoul_EventScript_DeclencherEpilogue,
// heart_and_soul_act5.inc) : consequences visibles des deux evenements.
// =======================================================================================
HeartSoul_EventScript_EpilogueOublies::
	call_if_set FLAG_VILLAGE_CACHE_DEFENDU, HeartSoul_EpilogueOublies_Village
	call_if_eq VAR_ARENES_LIBRES_JUGEMENT, 1, HeartSoul_EpilogueOublies_ArenesParrainees
	call_if_eq VAR_ARENES_LIBRES_JUGEMENT, 2, HeartSoul_EpilogueOublies_ArenesPerdues
	return

HeartSoul_EpilogueOublies_Village:
	goto_if_set FLAG_BULBIZARRE_EMMENE, HeartSoul_EpilogueOublies_VillageBulbi
	msgbox HeartSoul_Text_EpilogueOublies_VillageGardien, MSGBOX_DEFAULT
	return
HeartSoul_EpilogueOublies_VillageBulbi:
	msgbox HeartSoul_Text_EpilogueOublies_VillageBulbi, MSGBOX_DEFAULT
	return

HeartSoul_EpilogueOublies_ArenesParrainees:
	msgbox HeartSoul_Text_EpilogueOublies_ArenesParrainees, MSGBOX_DEFAULT
	return

HeartSoul_EpilogueOublies_ArenesPerdues:
	msgbox HeartSoul_Text_EpilogueOublies_ArenesPerdues, MSGBOX_DEFAULT
	return

HeartSoul_Text_EpilogueOublies_VillageGardien:
	.string "Au nord de la ROUTE 25, un village\n"
	.string "que personne ne connaît continue\l"
	.string "d'accueillir les POKÉMON oubliés.\p"
	.string "Un BULBIZARRE en garde l'entrée.\n"
	.string "Il n'a jamais laissé passer un\l"
	.string "seul collecteur.$"

HeartSoul_Text_EpilogueOublies_VillageBulbi:
	.string "MÉLANIE écrit parfois. Les POKÉMON\n"
	.string "de CINNABAR retrouvent leurs\l"
	.string "familles, un par un.\p"
	.string "“Dis à BULBIZARRE que le village\n"
	.string "tient bon. Il a bien choisi.”$"

HeartSoul_Text_EpilogueOublies_ArenesParrainees:
	.string "À CÉLADOPOLE, deux arènes de\n"
	.string "garage passent l'examen de la\l"
	.string "LIGUE. KAZ et YAS, côte à côte.\p"
	.string "Pour la première fois, le jury\n"
	.string "regarde les combats plutôt que\l"
	.string "les CV.$"

HeartSoul_Text_EpilogueOublies_ArenesPerdues:
	.string "À CÉLADOPOLE, le sous-sol du\n"
	.string "CASINO est muré. KAZ et YAS n'ont\l"
	.string "jamais été revus.\p"
	.string "On dit qu'ils se sont battus\n"
	.string "jusqu'au bout pour BLUE. Pour\l"
	.string "la seule licence qu'on leur\l"
	.string "ait jamais proposée.$"
