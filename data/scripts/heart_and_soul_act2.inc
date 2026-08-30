// Heart & Soul - Acte II : l'exil et la mefiance
// Sous-scripts appeles (call/return, pas goto/end) depuis les scripts existants des
// Champions de PEWTER (PierreDoute, dans PewterCity_Gym_hns/scripts.inc, EventScript_Brock)
// et CERULEAN (OndineDoute, dans CeruleanCity_Gym_hns/scripts.inc, EventScript_Misty).
// Le joueur est deja verrouille (lock/faceplayer) par le script appelant : ces sous-scripts
// ne font ni lock ni release, seulement des msgbox, comme HeartSoul_EventScript_PickOne_*.
//
// FLAG_PIERRE_CONVAINCU / FLAG_ONDINE_CONVAINCUE ne sont PAS poses par ces sous-scripts eux-
// memes (retour de test n°8 : les poser ici les figeait des la premiere tentative, meme en
// cas de defaite au combat qui suit - sur une deuxieme tentative apres une defaite, seul le
// dialogue anglais d'origine se relancait, plus le notre). Ils sont poses par
// HeartSoul_EventScript_PierreConvaincu / _OndineConvaincue, appeles depuis le script
// appelant UNIQUEMENT dans la branche de victoire (aux cotes de
// setflag FLAG_DEFEATED_PEWTER_GYM / FLAG_DEFEATED_CERULEAN_GYM) : la scene de doute se
// rejoue donc a chaque nouvelle tentative tant que le combat n'est pas gagne, ce qui colle
// mieux au texte ("Prouve-le sur le terrain") qu'un declenchement unique decorrele du
// resultat du combat.

HeartSoul_EventScript_PierreDoute::
	goto_if_unset FLAG_ACTE_1_TERMINE, HeartSoul_PierreDoute_Skip
	goto_if_set FLAG_PIERRE_CONVAINCU, HeartSoul_PierreDoute_Skip
	msgbox HeartSoul_Text_PierreDouteIntro, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_PierreDouteRecit, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_PierreDouteConclusion, MSGBOX_DEFAULT
	return
HeartSoul_PierreDoute_Skip:
	return

HeartSoul_EventScript_PierreConvaincu::
	setflag FLAG_PIERRE_CONVAINCU
	call_if_set FLAG_ONDINE_CONVAINCUE, HeartSoul_EventScript_ActeIITermine
	return

HeartSoul_EventScript_OndineDoute::
	goto_if_unset FLAG_ACTE_1_TERMINE, HeartSoul_OndineDoute_Skip
	goto_if_set FLAG_ONDINE_CONVAINCUE, HeartSoul_OndineDoute_Skip
	msgbox HeartSoul_Text_OndineDouteIntro, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_OndineDouteRecit, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_OndineDouteConclusion, MSGBOX_DEFAULT
	return
HeartSoul_OndineDoute_Skip:
	return

HeartSoul_EventScript_OndineConvaincue::
	setflag FLAG_ONDINE_CONVAINCUE
	call_if_set FLAG_PIERRE_CONVAINCU, HeartSoul_EventScript_ActeIITermine
	return

HeartSoul_EventScript_ActeIITermine::
	setflag FLAG_ACTE_2_TERMINE
	return

// Route 1, objet perdu (section 8 de histoire.md, point de reputation section 9). Appele en
// fin de Route1_EventScript_Quinn (data/maps/Route1_hns/scripts.inc), apres le message de
// post-combat existant, jamais avant trainerbattle_single (script declenche par la vue du
// dresseur, pas d'appel/msgbox prealable connu et teste dans ce fork pour ce cas - on ne
// prend pas ce risque, meme motif que les autres regles ON_TRANSITION deja documentees).
HeartSoul_EventScript_MiliceRoute1::
	goto_if_unset FLAG_ACTE_1_TERMINE, HeartSoul_MiliceRoute1_Skip
	goto_if_unset FLAG_OBJET_PERDU_TROUVE, HeartSoul_MiliceRoute1_Skip
	goto_if_set FLAG_OBJET_PERDU_RENDU, HeartSoul_MiliceRoute1_Skip
	msgbox HeartSoul_Text_MiliceIntro, MSGBOX_DEFAULT
HeartSoul_MiliceRoute1_Choice:
	dynmultipush HeartSoul_Text_RendreObjet, 0
	dynmultipush HeartSoul_Text_GarderObjet, 1
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, HeartSoul_MiliceRoute1_Rendre
	case 1, HeartSoul_MiliceRoute1_Garder
	goto HeartSoul_MiliceRoute1_Choice
HeartSoul_MiliceRoute1_Rendre:
	setflag FLAG_OBJET_PERDU_RENDU
	addvar VAR_REPUTATION, 1
	msgbox HeartSoul_Text_MiliceRendreResultat, MSGBOX_DEFAULT
	return
HeartSoul_MiliceRoute1_Garder:
	msgbox HeartSoul_Text_MiliceGarderResultat, MSGBOX_DEFAULT
	return
HeartSoul_MiliceRoute1_Skip:
	return

HeartSoul_Text_MiliceIntro:
	.string "QUINN: Attends. Ce bracelet, a ta\n"
	.string "ceinture… je le reconnais.\p"
	.string "Il appartient a une famille de\n"
	.string "CINNABAR qu'on a recueillie ici.$"

HeartSoul_Text_RendreObjet:
	.string "Le lui rendre$"

HeartSoul_Text_GarderObjet:
	.string "Le garder$"

HeartSoul_Text_MiliceRendreResultat:
	.string "QUINN: Merci. Ca va compter, pour\n"
	.string "eux comme pour nous.\p"
	.string "Passe, tu es attendue en ville.$"

HeartSoul_Text_MiliceGarderResultat:
	.string "QUINN: …\p"
	.string "Comme tu veux. Passe.$"

HeartSoul_Text_PierreDouteIntro:
	.string "PIERRE: Attends. Avant le combat,\n"
	.string "j'ai entendu ce qui se dit sur\l"
	.string "CINNABAR.\p"
	.string "Une ile entiere coupee du monde,\n"
	.string "et toi, sa Championne, en fuite…$"

HeartSoul_Text_PierreDouteRecit:
	.string "PIERRE: Tu n'as pas su proteger ta\n"
	.string "propre ville. Alors pourquoi je\l"
	.string "t'ecouterais parler d'une menace\l"
	.string "qui toucherait tout KANTO ?$"

HeartSoul_Text_PierreDouteConclusion:
	.string "…\p"
	.string "PIERRE: Prouve-le sur le terrain\n"
	.string "que je connais. Un combat, pour\l"
	.string "commencer.\p"
	.string "Apres, on reparlera de la TEAM\n"
	.string "ROCKET.$"

HeartSoul_Text_PierreApresVictoire:
	.string "PIERRE: D'accord. Tu tiens le\n"
	.string "choc, et tu n'as pas menti sur\l"
	.string "CINNABAR.\p"
	.string "Une Championne qui perd sa ville\n"
	.string "et qui vient quand meme se\l"
	.string "battre pour convaincre les\l"
	.string "autres, ca merite du respect.\p"
	.string "Si la TEAM ROCKET avance vers\n"
	.string "JADIELLE, je serai pret.$"

HeartSoul_Text_OndineDouteIntro:
	.string "ONDINE: Alors c'est vrai. CINNABAR\n"
	.string "est tombee.$"

HeartSoul_Text_OndineDouteRecit:
	.string "ONDINE: Une Championne qui perd\n"
	.string "son arene des le premier assaut…\l"
	.string "et qui vient maintenant me parler\l"
	.string "de reseau, de lieutenants, d'une\l"
	.string "menace sur tout KANTO ?$"

HeartSoul_Text_OndineDouteConclusion:
	.string "…\p"
	.string "ONDINE: Bats-toi. Si tu tiens le\n"
	.string "choc face a moi, j'ecouterai le\l"
	.string "reste.$"

// Viridian City, commercant resistant (section 8/9 de histoire.md). Appele depuis
// ViridianCity_Mart_EventScript_Cooltrainer (data/maps/ViridianCity_Mart_hns/scripts.inc),
// un PNJ existant a l'interieur du Mart (interaction classique, pas de trainerbattle - aucune
// restriction de placement comme pour HeartSoul_EventScript_MiliceRoute1).
HeartSoul_EventScript_CommercantViridian::
	goto_if_unset FLAG_ACTE_1_TERMINE, HeartSoul_CommercantViridian_Skip
	goto_if_set FLAG_COMMERCANT_VIRIDIAN_RESOLU, HeartSoul_CommercantViridian_Skip
	msgbox HeartSoul_Text_CommercantIntro, MSGBOX_DEFAULT
HeartSoul_CommercantViridian_Choice:
	dynmultipush HeartSoul_Text_AiderCommercant, 0
	dynmultipush HeartSoul_Text_IgnorerCommercant, 1
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, HeartSoul_CommercantViridian_Aider
	case 1, HeartSoul_CommercantViridian_Ignorer
	goto HeartSoul_CommercantViridian_Choice
HeartSoul_CommercantViridian_Aider:
	setflag FLAG_COMMERCANT_VIRIDIAN_RESOLU
	addvar VAR_REPUTATION, 1
	msgbox HeartSoul_Text_CommercantAiderResultat, MSGBOX_DEFAULT
	return
HeartSoul_CommercantViridian_Ignorer:
	setflag FLAG_COMMERCANT_VIRIDIAN_RESOLU
	msgbox HeartSoul_Text_CommercantIgnorerResultat, MSGBOX_DEFAULT
	return
HeartSoul_CommercantViridian_Skip:
	return

HeartSoul_Text_CommercantIntro:
	.string "…\p"
	.string "Ecoute. Depuis CINNABAR, certains\n"
	.string "ici pensent que la TEAM ROCKET\l"
	.string "'apporte de l'ordre', vu qu'il n'y\l"
	.string "a plus de Championne en ville.\p"
	.string "Moi, je finance une resistance\n"
	.string "discrete. J'ai besoin de gens qui\l"
	.string "remontent des informations.$"

HeartSoul_Text_AiderCommercant:
	.string "Aider a recolter des infos$"

HeartSoul_Text_IgnorerCommercant:
	.string "Decliner$"

HeartSoul_Text_CommercantAiderResultat:
	.string "Bien. Reviens me voir des que tu\n"
	.string "apprends quelque chose d'utile\l"
	.string "sur leurs mouvements.$"

HeartSoul_Text_CommercantIgnorerResultat:
	.string "…\p"
	.string "Comme tu veux. L'offre reste\n"
	.string "valable, si tu changes d'avis.$"

HeartSoul_Text_CommercantApresResolu:
	.string "Reste discrete. On ne sait jamais\n"
	.string "qui ecoute.$"

// Foret de Jade, bucherons deplaces (section 8/9 de histoire.md). Appele depuis
// Route2_EventScript_Doug (data/maps/Route2_hns/scripts.inc), objet-evenement place sur
// ViridianForest_hns mais defini dans ce fichier de scripts. Meme regle que
// HeartSoul_EventScript_MiliceRoute1 : insere apres le message de post-combat existant,
// jamais avant trainerbattle_single.
HeartSoul_EventScript_BucheronsForet::
	goto_if_unset FLAG_ACTE_1_TERMINE, HeartSoul_BucheronsForet_Skip
	goto_if_set FLAG_BUCHERONS_RESOLU, HeartSoul_BucheronsForet_Skip
	msgbox HeartSoul_Text_BucheronsIntro, MSGBOX_DEFAULT
HeartSoul_BucheronsForet_Choice:
	dynmultipush HeartSoul_Text_AiderBucherons, 0
	dynmultipush HeartSoul_Text_IgnorerBucherons, 1
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, HeartSoul_BucheronsForet_Aider
	case 1, HeartSoul_BucheronsForet_Ignorer
	goto HeartSoul_BucheronsForet_Choice
HeartSoul_BucheronsForet_Aider:
	setflag FLAG_BUCHERONS_RESOLU
	addvar VAR_REPUTATION, 1
	msgbox HeartSoul_Text_BucheronsAiderResultat, MSGBOX_DEFAULT
	return
HeartSoul_BucheronsForet_Ignorer:
	setflag FLAG_BUCHERONS_RESOLU
	msgbox HeartSoul_Text_BucheronsIgnorerResultat, MSGBOX_DEFAULT
	return
HeartSoul_BucheronsForet_Skip:
	return

HeartSoul_Text_BucheronsIntro:
	.string "…\p"
	.string "On coupait ce bois depuis des\n"
	.string "annees. La TEAM ROCKET a mis fin\l"
	.string "aux permis, d'un coup, sans rien\l"
	.string "proposer a la place.\p"
	.string "'Rendre la foret aux Pokemon',\n"
	.string "ils disent. Nous, on n'a plus\l"
	.string "rien pour nourrir nos familles.$"

HeartSoul_Text_AiderBucherons:
	.string "Leur laisser des vivres$"

HeartSoul_Text_IgnorerBucherons:
	.string "Continuer ta route$"

HeartSoul_Text_BucheronsAiderResultat:
	.string "Merci. Ca ne regle rien sur le\n"
	.string "fond, mais ca aide a tenir\l"
	.string "quelques jours de plus.$"

HeartSoul_Text_BucheronsIgnorerResultat:
	.string "…\p"
	.string "Ouais, c'est ca. File.$"

// Azuria (Cerulean), canalisations sabotees (section 8/9 de histoire.md). Appele depuis
// CeruleanCity_EventScript_Boy (data/maps/CeruleanCity_hns/scripts.inc), PNJ existant a
// interaction classique (msgbox MSGBOX_NPC), meme motif que
// HeartSoul_EventScript_CommercantViridian.
HeartSoul_EventScript_CanalisationsCerulean::
	goto_if_unset FLAG_ACTE_1_TERMINE, HeartSoul_CanalisationsCerulean_Skip
	goto_if_set FLAG_CANALISATIONS_REPAREES, HeartSoul_CanalisationsCerulean_Skip
	msgbox HeartSoul_Text_CanalisationsIntro, MSGBOX_DEFAULT
HeartSoul_CanalisationsCerulean_Choice:
	dynmultipush HeartSoul_Text_ReparerCanalisations, 0
	dynmultipush HeartSoul_Text_IgnorerCanalisations, 1
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, HeartSoul_CanalisationsCerulean_Reparer
	case 1, HeartSoul_CanalisationsCerulean_Ignorer
	goto HeartSoul_CanalisationsCerulean_Choice
HeartSoul_CanalisationsCerulean_Reparer:
	setflag FLAG_CANALISATIONS_REPAREES
	addvar VAR_REPUTATION, 1
	msgbox HeartSoul_Text_CanalisationsReparerResultat, MSGBOX_DEFAULT
	return
HeartSoul_CanalisationsCerulean_Ignorer:
	setflag FLAG_CANALISATIONS_REPAREES
	msgbox HeartSoul_Text_CanalisationsIgnorerResultat, MSGBOX_DEFAULT
	return
HeartSoul_CanalisationsCerulean_Skip:
	return

HeartSoul_Text_CanalisationsIntro:
	.string "…\p"
	.string "La TEAM ROCKET a sabote nos\n"
	.string "canalisations avant de repartir.\l"
	.string "L'ARENE d'ONDINE sert de poste de\l"
	.string "secours, en attendant.\p"
	.string "Un coup de main pour les\n"
	.string "reparations, ca compterait pour\l"
	.string "elle.$"

HeartSoul_Text_ReparerCanalisations:
	.string "Donner un coup de main$"

HeartSoul_Text_IgnorerCanalisations:
	.string "Passer ton chemin$"

HeartSoul_Text_CanalisationsReparerResultat:
	.string "Merci. Avec toi, ca ira plus\n"
	.string "vite. ONDINE pourra rouvrir son\l"
	.string "ARENE normalement.$"

HeartSoul_Text_CanalisationsIgnorerResultat:
	.string "…\p"
	.string "Bon, tant pis. On se debrouillera.$"

HeartSoul_Text_CanalisationsApres:
	.string "Grace a toi, les canalisations\n"
	.string "tiennent de nouveau.$"

// Azuria - Arene fermee tant que les canalisations ne sont pas reparees (retour de test n°11).
// La TEAM ROCKET (l'objet-evenement LOCALID_CERULEAN_GYM_ROCKET, deja present dans ce fork
// mais sans aucun script d'interaction - "script": NULL dans map.json - vraisemblablement un
// reste de generation jamais complete) est le sabotage a l'origine du probleme. Reutilise
// plutot que d'ajouter un nouveau PNJ.
HeartSoul_EventScript_OndineAvantCanalisations::
	msgbox HeartSoul_Text_OndineAvantCanalisationsIntro, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_OndineAvantCanalisationsDemande, MSGBOX_DEFAULT
	return

HeartSoul_Text_OndineAvantCanalisationsIntro:
	.string "ONDINE: N'approche pas plus.\n"
	.string "L'ARENE est hors service.\p"
	.string "Un type de la TEAM ROCKET a\n"
	.string "sabote nos canalisations avant\l"
	.string "de filer. Je gere l'inondation,\l"
	.string "pas le temps de me battre.$"

HeartSoul_Text_OndineAvantCanalisationsDemande:
	.string "ONDINE: Si tu veux vraiment que\n"
	.string "je t'ecoute parler de reseau\l"
	.string "ROCKET, commence par m'aider a\l"
	.string "reparer ce gachis en ville.\p"
	.string "Apres, on parlera.$"

HeartSoul_EventScript_RocketGruntCerulean::
	lock
	faceplayer
	msgbox HeartSoul_Text_RocketGruntCerulean, MSGBOX_DEFAULT
	release
	end

HeartSoul_Text_RocketGruntCerulean:
	.string "Alors, elle est belle ma\n"
	.string "petite inondation ?\p"
	.string "Tu peux toujours essayer de\n"
	.string "reparer ca, ca ne rouvrira pas\l"
	.string "cette ARENE de sitot.$"

// Declenche depuis CeruleanCity_Gym_OnFrame (map_script_2 VAR_TEMP_1, 0), meme motif que
// CeruleanCity_Gym_EventScript_TryMachinePart juste au-dessus dans le meme fichier (condition
// toujours vraie, le sous-script decide lui-meme via goto_if_set/unset). Fait disparaitre le
// Rocket une fois les canalisations reparees, sans toucher a VAR_KANTO_ROCKET_STORY_STATE
// (systeme du jeu de base, non modifie).
HeartSoul_EventScript_CeruleanGymRocketCheck::
	goto_if_unset FLAG_CANALISATIONS_REPAREES, HeartSoul_CeruleanGymRocketCheck_End
	goto_if_set FLAG_CERULEAN_GYM_ROCKET_PARTI, HeartSoul_CeruleanGymRocketCheck_End
	setflag FLAG_HIDE_CERULEAN_GYM_ROCKET
	removeobject LOCALID_CERULEAN_GYM_ROCKET
	setflag FLAG_CERULEAN_GYM_ROCKET_PARTI
HeartSoul_CeruleanGymRocketCheck_End:
	end

// Pallet Town, "le monde d'avant" (section 8 de histoire.md). PNJ existant reflavore
// (PalletTown_EventScript_Woman, texte d'origine deja retire). Pas de choix ni de point de
// reputation : narration seule, comme prevu par le brief ("recroise en Acte III/IV").
HeartSoul_EventScript_PalletJeuneDresseur::
	goto_if_unset FLAG_ACTE_1_TERMINE, HeartSoul_PalletJeuneDresseur_Skip
	goto_if_set FLAG_PALLET_JEUNE_DRESSEUR_RESOLU, HeartSoul_PalletJeuneDresseur_Apres
	msgbox HeartSoul_Text_PalletJeuneDresseurIntro, MSGBOX_NPC
	setflag FLAG_PALLET_JEUNE_DRESSEUR_RESOLU
	return
HeartSoul_PalletJeuneDresseur_Apres:
	msgbox HeartSoul_Text_PalletJeuneDresseurApres, MSGBOX_NPC
	return
HeartSoul_PalletJeuneDresseur_Skip:
	return

HeartSoul_Text_PalletJeuneDresseurIntro:
	.string "Cinnabar ? On en parle, oui…\n"
	.string "mais bon, ici tout va bien.\p"
	.string "Vous vous en faites toujours\n"
	.string "trop, vous les gens du sud.$"

HeartSoul_Text_PalletJeuneDresseurApres:
	.string "J'espere que tu retrouveras\n"
	.string "vite BLAINE.$"

// Vermilion, doute de Major Bob avant le combat de badge (meme motif que Pierre/Ondine,
// meme correctif retour de test n°8 : le flag n'est pose qu'a la victoire, jamais dans ce
// sous-script, pour que la scene se rejoue tant que le combat n'est pas gagne).
HeartSoul_EventScript_MajorBobDoute::
	goto_if_unset FLAG_ACTE_1_TERMINE, HeartSoul_MajorBobDoute_Skip
	goto_if_set FLAG_MAJORBOB_CONVAINCU, HeartSoul_MajorBobDoute_Skip
	msgbox HeartSoul_Text_MajorBobDouteIntro, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_MajorBobDouteRecit, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_MajorBobDouteConclusion, MSGBOX_DEFAULT
	return
HeartSoul_MajorBobDoute_Skip:
	return

HeartSoul_EventScript_MajorBobConvaincu::
	setflag FLAG_MAJORBOB_CONVAINCU
	return

HeartSoul_Text_MajorBobDouteIntro:
	.string "MAJOR BOB: Alors comme ca c'est\n"
	.string "toi, la Championne de CINNABAR.$"

HeartSoul_Text_MajorBobDouteRecit:
	.string "MAJOR BOB: On m'a parle d'un\n"
	.string "reseau ROCKET, de lieutenants…\l"
	.string "Facile a dire. Difficile a\l"
	.string "prouver.$"

HeartSoul_Text_MajorBobDouteConclusion:
	.string "…\p"
	.string "MAJOR BOB: Montre-moi ce que tu\n"
	.string "vaux au combat. On en reparlera\l"
	.string "apres.$"

// Azuria->Vermilion, interception radio (section 8/9). PNJ existant reflavore
// (VermilionCity_EventScript_Nerd). Simplification volontaire, documentee : le brief decrit
// une "sequence d'ecoute/puzzle" ; sans mini-jeu existant a reutiliser dans ce fork pour ca,
// implemente comme un choix simple (aider/ignorer), meme structure que les autres
// sous-intrigues, plutot que d'inventer un mini-jeu risque sans verification possible.
HeartSoul_EventScript_InterceptionRadio::
	goto_if_unset FLAG_ACTE_1_TERMINE, HeartSoul_InterceptionRadio_Skip
	goto_if_set FLAG_INTERCEPTION_RADIO_RESOLUE, HeartSoul_InterceptionRadio_Skip
	msgbox HeartSoul_Text_InterceptionRadioIntro, MSGBOX_DEFAULT
HeartSoul_InterceptionRadio_Choice:
	dynmultipush HeartSoul_Text_AiderRadio, 0
	dynmultipush HeartSoul_Text_IgnorerRadio, 1
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, HeartSoul_InterceptionRadio_Aider
	case 1, HeartSoul_InterceptionRadio_Ignorer
	goto HeartSoul_InterceptionRadio_Choice
HeartSoul_InterceptionRadio_Aider:
	setflag FLAG_INTERCEPTION_RADIO_RESOLUE
	addvar VAR_REPUTATION, 1
	msgbox HeartSoul_Text_InterceptionRadioAiderResultat, MSGBOX_DEFAULT
	return
HeartSoul_InterceptionRadio_Ignorer:
	setflag FLAG_INTERCEPTION_RADIO_RESOLUE
	msgbox HeartSoul_Text_InterceptionRadioIgnorerResultat, MSGBOX_DEFAULT
	return
HeartSoul_InterceptionRadio_Skip:
	return

HeartSoul_Text_InterceptionRadioIntro:
	.string "…\p"
	.string "MAJOR BOB me fait intercepter\n"
	.string "les transmissions radio de la\l"
	.string "TEAM ROCKET. J'ai besoin d'un\l"
	.string "coup de main pour trier tout ca.$"

HeartSoul_Text_AiderRadio:
	.string "Aider a trier les transmissions$"

HeartSoul_Text_IgnorerRadio:
	.string "Laisser faire$"

HeartSoul_Text_InterceptionRadioAiderResultat:
	.string "Merci. On a repere des mouvements\n"
	.string "suspects du cote de SAFRANIA.\p"
	.string "Ca servira, pour la suite.$"

HeartSoul_Text_InterceptionRadioIgnorerResultat:
	.string "…\p"
	.string "Tant pis, je me debrouillerai.$"

HeartSoul_Text_InterceptionRadioApres:
	.string "Toujours rien de neuf sur les\n"
	.string "frequences ROCKET.$"
