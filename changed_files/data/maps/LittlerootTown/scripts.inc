LittlerootTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LittlerootTown_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, LittlerootTown_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, LittlerootTown_OnWarp
	.byte 0

	@ State descriptions for VAR_LITTLEROOT_TOWN_STATE
	@ 1: Met Rival
	@ 2: A reçu PIKACHU du Professeur Chen au labo (Kanto Saison 1 : plus de scène de sauvetage sur
	@    la Route 1, cf. CHANGELOG — cette valeur servait avant à "Saving Birch", elle sert
	@    maintenant seulement à lever le blocage de la Jumelle à la sortie nord tant qu'on n'a pas
	@    de POKéMON)
	@ 3: Received Pokedex
	@ 4: Received Running Shoes

	@ State descriptions for VAR_LITTLEROOT_INTRO_STATE (Kanto Saison 1 : plus de camion/emménagement,
	@ le joueur habite déjà Bourg Palette, cf. Episode 1.1)
	@ 0: Pas encore parlé à Maman ce matin
	@ 1: A parlé à Maman, direction le labo

	@ State descriptions for VAR_LITTLEROOT_HOUSES_STATE_MAY
	@                    and VAR_LITTLEROOT_HOUSES_STATE_BRENDAN
	@ Hérité de la structure Hoenn (rencontre voisin/déménagement) — plus jamais mis à 1/2 depuis la
	@ suppression du camion, seuls les états 3/4/5 (SS Ticket/Latios, contenu post-Ligue) restent
	@ atteignables via Hall of Fame. Contenu "papa" non traité cette session (cf. CHANGELOG).
	@ 1: On Truck (and is corresponding gender) — mort, plus jamais atteint
	@ 2: Met Rival's Mom (and is corresponding gender) — mort, plus jamais atteint
	@ 3: Entered Hall of Fame
	@ 4: Completed Lati TV / SS Ticket event
	@ 5: Exited house after above event

	@ State descriptions for VAR_LITTLEROOT_RIVAL_STATE
	@ 1: Never occurs
	@ 2: Entered Rival's bedroom
	@ 3: Met Rival
	@ 4: Received Pokedex


LittlerootTown_OnTransition:
	setflag FLAG_VISITED_LITTLEROOT_TOWN
	call Custom_EventScript_SetupRegisGfxId
	call_if_unset FLAG_RESCUED_BIRCH, LittlerootTown_EventScript_SetTwinPos
	call_if_eq VAR_LITTLEROOT_TOWN_STATE, 3, LittlerootTown_EventScript_SetMomStandingInFrontOfDoorPos
	call_if_eq VAR_LITTLEROOT_HOUSES_STATE_MAY, 4, LittlerootTown_EventScript_SetExitedHouseAfterLatiSSTicketEvent
	call_if_eq VAR_LITTLEROOT_HOUSES_STATE_BRENDAN, 4, LittlerootTown_EventScript_SetExitedHouseAfterLatiSSTicketEvent
	call_if_eq VAR_OLDALE_RIVAL_STATE, 1, LittlerootTown_EventScript_MoveRivalFromOldale
	call_if_eq VAR_LITTLEROOT_RIVAL_STATE, 3, LittlerootTown_EventScript_SetRivalLeftForRoute103
	call_if_eq VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 1, LittlerootTown_EventScript_HideMapNamePopup
	call_if_eq VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 2, LittlerootTown_EventScript_LeftLabAfterDexUpgrade
	end

LittlerootTown_EventScript_LeftLabAfterDexUpgrade::
	setvar VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 3
	return

LittlerootTown_EventScript_HideMapNamePopup::
	setflag FLAG_HIDE_MAP_NAME_POPUP
	return

LittlerootTown_EventScript_SetRivalLeftForRoute103::
	setflag FLAG_RIVAL_LEFT_FOR_ROUTE103
	return

LittlerootTown_EventScript_MoveRivalFromOldale::
	setvar VAR_OLDALE_RIVAL_STATE, 2
	setflag FLAG_HIDE_OLDALE_TOWN_RIVAL
	return

LittlerootTown_EventScript_SetExitedHouseAfterLatiSSTicketEvent::
	setvar VAR_LITTLEROOT_HOUSES_STATE_MAY, 5
	setvar VAR_LITTLEROOT_HOUSES_STATE_BRENDAN, 5
	return

LittlerootTown_EventScript_SetTwinPos::
	goto_if_eq VAR_LITTLEROOT_TOWN_STATE, 0, LittlerootTown_EventScript_SetTwinGuardingRoutePos
	setobjectxyperm LOCALID_LITTLEROOT_TWIN, 18, 1
	setobjectmovementtype LOCALID_LITTLEROOT_TWIN, MOVEMENT_TYPE_FACE_UP
	return

LittlerootTown_EventScript_SetTwinGuardingRoutePos::
	setobjectxyperm LOCALID_LITTLEROOT_TWIN, 17, 2
	setobjectmovementtype LOCALID_LITTLEROOT_TWIN, MOVEMENT_TYPE_FACE_DOWN
	return

LittlerootTown_OnFrame:
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 1, LittlerootTown_EventScript_BeginDexUpgradeScene
	.2byte 0

LittlerootTown_EventScript_BeginDexUpgradeScene::
	lockall
	playse SE_PIN
	applymovement LOCALID_LITTLEROOT_BIRCH, Common_Movement_ExclamationMark
	waitmovement 0
	delay 80
	msgbox LittlerootTown_Text_BirchSomethingToShowYouAtLab, MSGBOX_DEFAULT
	closemessage
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_RIVAL
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BIRCH
	delay 20
	clearflag FLAG_HIDE_MAP_NAME_POPUP
	warp MAP_LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB, 6, 5
	waitstate
	releaseall
	end

LittlerootTown_OnWarp:
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 1, LittlerootTown_EventScript_SetRivalBirchPosForDexUpgrade
	.2byte 0

LittlerootTown_EventScript_SetRivalBirchPosForDexUpgrade::
	addobject LOCALID_LITTLEROOT_BIRCH
	addobject LOCALID_LITTLEROOT_RIVAL
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_SetRivalBirchPosForDexUpgradeMale
	goto LittlerootTown_EventScript_SetRivalBirchPosForDexUpgradeFemale
	end

LittlerootTown_EventScript_SetRivalBirchPosForDexUpgradeMale::
	setobjectxy LOCALID_LITTLEROOT_RIVAL, 10, 9
	setobjectxy LOCALID_LITTLEROOT_BIRCH, 11, 9
	end

LittlerootTown_EventScript_SetRivalBirchPosForDexUpgradeFemale::
	setobjectxy LOCALID_LITTLEROOT_RIVAL, 10, 9
	setobjectxy LOCALID_LITTLEROOT_BIRCH, 11, 9
	end

LittlerootTown_EventScript_FatMan::
	msgbox LittlerootTown_Text_CanUsePCToStoreItems, MSGBOX_NPC
	end

LittlerootTown_EventScript_Boy::
	msgbox LittlerootTown_Text_BirchSpendsDaysInLab, MSGBOX_NPC
	end

@ Kanto Saison 1 : plus de scène de sauvetage du Professeur Chen (cf. CHANGELOG) — la Jumelle se
@ contente maintenant d'indiquer le chemin du labo tant qu'on n'a pas encore de POKéMON.
LittlerootTown_EventScript_Twin::
	lock
	faceplayer
	goto_if_set FLAG_ADVENTURE_STARTED, LittlerootTown_EventScript_GoodLuck
	goto_if_set FLAG_RESCUED_BIRCH, LittlerootTown_EventScript_YouSavedBirch
	msgbox LittlerootTown_Text_IfYouGoInGrassPokemonWillJumpOut, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_YouSavedBirch::
	special GetPlayerBigGuyGirlString
	msgbox LittlerootTown_Text_YouSavedBirch, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_GoodLuck::
	msgbox LittlerootTown_Text_GoodLuckCatchingPokemon, MSGBOX_DEFAULT
	release
	end

@ Kanto Saison 1 : la scène de sauvetage du Professeur Chen sur la Route 1 (et la remise du PIKACHU
@ qui suivait) a été entièrement supprimée sur consigne de Thomas — le joueur reçoit maintenant
@ PIKACHU directement en discutant avec le Professeur Chen dans son laboratoire
@ (LittlerootTown_ProfessorBirchsLab_EventScript_GiveDirectPikachu, cf. CHANGELOG). Cette scène avait
@ été déplacée ici en suite 20 pour contourner un bug moteur dans applymovement (cf. commit précédent
@ pour le détail), mais le nouveau design l'élimine complètement au lieu de le contourner.

@ Kanto Saison 1 (suite 26) : sur nouvelle demande de Thomas, la Jumelle (Twin) ne bloque plus jamais
@ la sortie nord vers la Route 1 — l'ancien verrou (LittlerootTown_EventScript_NeedPokemonTrigger{Left,
@ Right}, qui la faisait accourir pour repousser le joueur tant qu'il n'avait pas de POKéMON) et toute
@ sa machinerie de déplacement associée ont été entièrement supprimés, ainsi que les coord_events qui
@ les déclenchaient dans map.json. Twin reste présente ailleurs en ville (PNJ normal, cf. object_events)
@ mais ne bloque plus aucun passage.

LittlerootTown_EventScript_TownSign::
	msgbox LittlerootTown_Text_TownSign, MSGBOX_SIGN
	end

LittlerootTown_EventScript_BirchsLabSign::
	msgbox LittlerootTown_Text_ProfBirchsLab, MSGBOX_SIGN
	end

LittlerootTown_EventScript_BrendansHouseSign::
	lockall
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_PlayersHouseSignMale
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_EventScript_BirchsHouseSignFemale
	releaseall
	end

LittlerootTown_EventScript_PlayersHouseSignMale::
	msgbox LittlerootTown_Text_PlayersHouse, MSGBOX_DEFAULT
	return

LittlerootTown_EventScript_BirchsHouseSignFemale::
	msgbox LittlerootTown_Text_ProfBirchsHouse, MSGBOX_DEFAULT
	return

LittlerootTown_EventScript_MaysHouseSign::
	lockall
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_BirchsHouseSignMale
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_EventScript_PlayersHouseSignFemale
	releaseall
	end

LittlerootTown_EventScript_BirchsHouseSignMale::
	msgbox LittlerootTown_Text_ProfBirchsHouse, MSGBOX_DEFAULT
	return

LittlerootTown_EventScript_PlayersHouseSignFemale::
	msgbox LittlerootTown_Text_PlayersHouse, MSGBOX_DEFAULT
	return

@ Kanto Saison 1 (suite 30) : scène des cadeaux de Maman réécrite pour se dérouler devant le Labo,
@ juste après la visite au Professeur Chen (VAR_LITTLEROOT_TOWN_STATE == 3, positionné dès l'entrée
@ sur la carte via LittlerootTown_EventScript_SetMomStandingInFrontOfDoorPos, appelé depuis
@ LittlerootTown_OnTransition). Maman se tient à droite de la porte du Labo et remet, en une seule
@ visite, les Chaussures de Course, le Vélo et la Canne à Pêche.
LittlerootTown_EventScript_SetMomStandingInFrontOfDoorPos::
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_MOM_OUTSIDE
	setobjectmovementtype LOCALID_LITTLEROOT_MOM, MOVEMENT_TYPE_FACE_DOWN
	setobjectxyperm LOCALID_LITTLEROOT_MOM, 26, 8
	return

LittlerootTown_EventScript_Mom::
	lock
	faceplayer
	goto_if_eq VAR_LITTLEROOT_TOWN_STATE, 3, LittlerootTown_EventScript_MomGiveGiftsOutsideLab
	msgbox LittlerootTown_Text_MomGenericChat, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_MomGiveGiftsOutsideLab::
	msgbox LittlerootTown_Text_WearTheseRunningShoes, MSGBOX_DEFAULT
	call LittlerootTown_EventScript_GiveRunningShoes
	call LittlerootTown_EventScript_GiveBicycle
	call LittlerootTown_EventScript_GiveFishingRod
	msgbox LittlerootTown_Text_ComeHomeIfAnythingHappens, MSGBOX_DEFAULT
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MOM_OUTSIDE
	setvar VAR_LITTLEROOT_TOWN_STATE, 4
	release
	end

LittlerootTown_EventScript_GiveRunningShoes::
	playfanfare MUS_OBTAIN_ITEM
	message LittlerootTown_Text_SwitchShoesWithRunningShoes
	waitfanfare
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox LittlerootTown_Text_ExplainRunningShoes, MSGBOX_DEFAULT
	closemessage
	delay 20
	return

LittlerootTown_EventScript_GiveBicycle::
	giveitem_msg LittlerootTown_Text_ObtainedBicycle, ITEM_MACH_BIKE, 1, MUS_OBTAIN_ITEM
	closemessage
	delay 20
	return

LittlerootTown_EventScript_GiveFishingRod::
	giveitem_msg LittlerootTown_Text_ObtainedFishingRod, ITEM_OLD_ROD, 1, MUS_OBTAIN_ITEM
	setflag FLAG_RECEIVED_OLD_ROD
	closemessage
	delay 20
	return

LittlerootTown_Text_WearTheseRunningShoes:
	.string "MAMAN : {PLAYER} ! {PLAYER} !\n"
	.string "Tu t'es bien présenté(e) au\l"
	.string "Professeur Chen ?\p"
	.string "Oh ! Quel adorable POKéMON !\n"
	.string "Tu l'as eu grâce au Professeur Chen.\l"
	.string "Comme c'est gentil de sa part !\p"
	.string "Tu es vraiment digne de ton père,\n"
	.string "tu vas bien avec les POKéMON !\p"
	.string "Tiens, mon cœur ! Puisque tu pars à\n"
	.string "l'aventure, prends aussi tout ça.$"

LittlerootTown_Text_SwitchShoesWithRunningShoes:
	.string "{PLAYER} a échangé ses chaussures\n"
	.string "contre les CHAUSSURES DE COURSE.$"

LittlerootTown_Text_ExplainRunningShoes:
	.string "MAMAN : {PLAYER}, ces chaussures\n"
	.string "sont livrées avec un mode d'emploi.\p"
	.string "“Maintiens le bouton B en portant ces\n"
	.string "CHAUSSURES DE COURSE pour courir\l"
	.string "bien plus vite !\p"
	.string "Enfile ces CHAUSSURES DE COURSE\n"
	.string "et fonce à travers la nature !”$"

LittlerootTown_Text_ObtainedBicycle:
	.string "MAMAN : Et prends aussi ce VÉLO !\p"
	.string "Il ira parfaitement avec tes nouvelles\n"
	.string "chaussures pour explorer plus vite.$"

LittlerootTown_Text_ObtainedFishingRod:
	.string "MAMAN : Et enfin, cette CANNE À\n"
	.string "PÊCHE !\p"
	.string "Ton père l'utilisait beaucoup, elle\n"
	.string "pourrait bien te servir en chemin.$"

LittlerootTown_Text_ComeHomeIfAnythingHappens:
	.string "… … … … … … … …\n"
	.string "… … … … … … … …\p"
	.string "Dire que tu as maintenant ton propre\n"
	.string "POKéMON…\l"
	.string "Ton père va être fou de joie.\p"
	.string "…Mais fais bien attention.\n"
	.string "S'il arrive quoi que ce soit, tu peux\l"
	.string "toujours rentrer à la maison.\p"
	.string "Allez, va les chercher, mon cœur !$"

LittlerootTown_Text_MomGenericChat:
	.string "MAMAN : Fais attention à toi, et\n"
	.string "n'oublie pas d'appeler de temps en\l"
	.string "temps !$"

LittlerootTown_Text_CanUsePCToStoreItems:
	.string "If you use a PC, you can store items\n"
	.string "and POKéMON.\p"
	.string "The power of science is staggering!$"

LittlerootTown_Text_BirchSpendsDaysInLab:
	.string "PROF. BIRCH spends days in his LAB\n"
	.string "studying, then he'll suddenly go out in\l"
	.string "the wild to do more research…\p"
	.string "When does PROF. BIRCH spend time\n"
	.string "at home?$"

LittlerootTown_Text_IfYouGoInGrassPokemonWillJumpOut:
	.string "Eum, eum, eum !\p"
	.string "Si tu sors et que tu marches dans\n"
	.string "les hautes herbes, des POKéMON\l"
	.string "sauvages vont surgir !\p"
	.string "Tu devrais d'abord aller voir le\n"
	.string "Professeur Chen à son labo, il\l"
	.string "pourra sûrement t'aider.$"

LittlerootTown_Text_YouSavedBirch:
	.string "Alors, PIKACHU te plaît ?\p"
	.string "J'ai hâte de voir comment vous\n"
	.string "vous entendez tous les deux !$"

LittlerootTown_Text_GoodLuckCatchingPokemon:
	.string "Tu vas attraper des POKéMON ?\n"
	.string "Bonne chance !$"

LittlerootTown_Text_TownSign:
	.string "LITTLEROOT TOWN\n"
	.string "“A town that can't be shaded any hue.”$"

LittlerootTown_Text_ProfBirchsLab:
	.string "PROF. BIRCH'S POKéMON LAB$"

LittlerootTown_Text_PlayersHouse:
	.string "{PLAYER}'s HOUSE$"

LittlerootTown_Text_ProfBirchsHouse:
	.string "PROF. BIRCH'S HOUSE$"

LittlerootTown_Text_BirchSomethingToShowYouAtLab:
	.string "PROF. BIRCH: Well, well, {PLAYER}{KUN}!\n"
	.string "That was good work out there!\p"
	.string "I knew there was something special\n"
	.string "about you when I first saw you,\l"
	.string "but I never expected this.\p"
	.string "Oh, yes. Do you still have the POKéDEX\n"
	.string "I gave you?\p"
	.string "I have something to show you.\n"
	.string "Let's go to my LAB.$"

@ Kanto Saison 1 : panneaux des 5 nouvelles maisons du quartier sud de Bourg Palette (cf. CHANGELOG).
LittlerootTown_EventScript_MmeChenHouseSign::
	msgbox LittlerootTown_Text_MmeChenHouseSign, MSGBOX_SIGN
	end

LittlerootTown_EventScript_VieuxDresseurHouseSign::
	msgbox LittlerootTown_Text_VieuxDresseurHouseSign, MSGBOX_SIGN
	end

LittlerootTown_EventScript_GardienRouteHouseSign::
	msgbox LittlerootTown_Text_GardienRouteHouseSign, MSGBOX_SIGN
	end

LittlerootTown_EventScript_VoletsFermesHouseSign::
	msgbox LittlerootTown_Text_VoletsFermesHouseSign, MSGBOX_SIGN
	end

LittlerootTown_EventScript_DameBaiesHouseSign::
	msgbox LittlerootTown_Text_DameBaiesHouseSign, MSGBOX_SIGN
	end

LittlerootTown_Text_MmeChenHouseSign:
	.string "MAISON DE MME CHEN$"

LittlerootTown_Text_VieuxDresseurHouseSign:
	.string "MAISON DU VIEUX DRESSEUR$"

LittlerootTown_Text_GardienRouteHouseSign:
	.string "MAISON DU GARDIEN DE ROUTE$"

LittlerootTown_Text_VoletsFermesHouseSign:
	.string "??? (volets fermés)$"

LittlerootTown_Text_DameBaiesHouseSign:
	.string "MAISON DE LA DAME AUX BAIES$"

