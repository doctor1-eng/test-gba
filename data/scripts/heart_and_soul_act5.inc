// Heart & Soul - Acte V : confrontation finale et epilogues (docs/heart_and_soul/docs/
// histoire.md section 2 et 10). Adapte depuis les templates
// docs/heart_and_soul/scripts/blue_final.pory et epilogues.pory (syntaxe Poryscript non
// utilisee dans ce fork - voir implementation_notes.md "Convention de scripting reelle" -
// traduit en macros natives, meme convention que le reste du projet).
//
// Decision utilisateur deja actee par une session precedente (implementation_notes.md,
// "Resolution : Blue masque a l'Arene d'Argenta") : la confrontation finale avec Blue a lieu
// a l'Arene de Viridian (ViridianCity_Gym_hns), pas au Pokemon Mansion de Cinnabar comme le
// suggerait initialement histoire.md - Blue y est deja cache derriere FLAG_HIDE_VIRIDIAN_BLUE
// jusqu'a FLAG_ACTE_5_DEBLOQUE, TRAINER_BLUE_HNS deja au niveau 34, badge/TM deja fonctionnels
// dans le jeu de base. Cette session ajoute uniquement la sequence de dialogue a choix
// multiples (histoire.md section 3, "Blue - motivation et scene de revelation") et le
// declenchement des epilogues, sans toucher aux mecaniques de badge deja en place.

// ---------------------------------------------------------------------------------------
// Verrou d'Acte V : ne s'active que si TOUS les objectifs d'Acte IV sont remplis (alliance
// complete des Champions + Blaine retrouve + dernier lieutenant vaincu, histoire.md section
// 2). Appele en call/return depuis les 5 points de victoire concernes (Erika/Sabrina/Janine/
// Blaine/Mira Voss - Major Bob deja fait en Acte II, verifie ici seulement). Poser
// FLAG_ACTE_5_DEBLOQUE suffit a tout debloquer : la carte Cinnabar
// (HeartSoul_EventScript_CinnabarVerrouilleeCheck, heart_and_soul_act1.inc) et l'apparition
// de Blue a Viridian (ViridianCity_Gym_OnTransition) verifient deja ce meme flag - aucune
// autre modification necessaire de ce cote.
// ---------------------------------------------------------------------------------------
HeartSoul_EventScript_VerifierDeblocageActeV::
	goto_if_set FLAG_ACTE_5_DEBLOQUE, HeartSoul_VerifierActeV_Skip
	goto_if_unset FLAG_ERIKA_CONVAINCUE, HeartSoul_VerifierActeV_Skip
	goto_if_unset FLAG_SABRINA_CONVAINCUE, HeartSoul_VerifierActeV_Skip
	goto_if_unset FLAG_JANINE_CONVAINCUE, HeartSoul_VerifierActeV_Skip
	goto_if_unset FLAG_MAJORBOB_CONVAINCU, HeartSoul_VerifierActeV_Skip
	goto_if_unset FLAG_BLAINE_SAUVE, HeartSoul_VerifierActeV_Skip
	goto_if_unset FLAG_MIRA_VOSS_VAINCUE, HeartSoul_VerifierActeV_Skip
	setflag FLAG_ACTE_5_DEBLOQUE
	return
HeartSoul_VerifierActeV_Skip:
	return

// ---------------------------------------------------------------------------------------
// Confrontation Blue (Arene de Viridian). Meme motif que toutes les scenes de doute : call/
// return depuis ViridianCity_Gym_EventScript_Blue, avant le msgbox/trainerbattle_no_intro
// d'origine (non touches). Sequence rejouee a chaque tentative tant que le combat n'est pas
// gagne - meme comportement que Pierre/Ondine/Erika/Sabrina/Janine (le score de la sequence
// est donc recalcule a chaque essai, pas de garde anti-repetition necessaire ici,
// contrairement a Terrence/Kess qui ont 3 issues differentes a proteger).
//
// Meme piege de decalage que Terrence/Kess (VAR_DIALOGUE_BLUE 16 bits non signe) : le
// bareme d'origine (histoire.md section 3) va de -2 a +2 par reponse, decalage de +2 par
// question (au lieu de +1 pour Terrence/Kess, car le minimum ici est -2 et pas -1) :
// -2/0/+2 deviennent 0/2/4, -1/+1 deviennent 1/3. Seuils recalcules (total original ->
// decale, +6 sur 3 questions) : original >=3 -> decale >=9 ; 0 a 2 -> decale 6-8 ; negatif
// -> decale <=5.
// Evenement 18 (histoire.md section 11) : "juste avant la confrontation avec Blue, les
// lieutenants convertis (Terrence, Kess) et les Champions ralliés apparaissent brièvement pour
// sécuriser l'accès au Mansion". Implemente en un seul msgbox recapitulatif plutot qu'en
// object_events additionnels sur ViridianCity_Gym_hns (la piece est deja occupee par le
// combat scripte de Blue - ajouter des sprites supplementaires risquerait de gener le
// placement du trainerbattle_no_intro d'origine, non touche par ce chantier). Le texte varie
// legerement selon qui a effectivement ete convaincu/vaincu (FLAG_TERRENCE_CONVAINCU,
// FLAG_KESS_CONVAINCUE), pour que la scene "materialise concretement le score de reputation
// accumule" comme demande, sans dupliquer le calcul complet des epilogues plus bas dans ce
// fichier. Guarde par FLAG_RETOUR_ALLIES_VU pour ne s'afficher qu'une fois, meme si le joueur
// perd le combat de Blue et retente la sequence.
HeartSoul_EventScript_BlueConfrontation::
	goto_if_set FLAG_RETOUR_ALLIES_VU, HeartSoul_BlueConfrontation_ApresRetourAllies
	goto_if_set FLAG_TERRENCE_CONVAINCU, HeartSoul_RetourAllies_CheckKessAvecTerrence
	goto_if_set FLAG_KESS_CONVAINCUE, HeartSoul_RetourAllies_KessSeule
	msgbox HeartSoul_Text_RetourAlliesChampionsSeuls, MSGBOX_DEFAULT
	goto HeartSoul_RetourAllies_Fin
HeartSoul_RetourAllies_CheckKessAvecTerrence:
	goto_if_set FLAG_KESS_CONVAINCUE, HeartSoul_RetourAllies_TerrenceEtKess
	msgbox HeartSoul_Text_RetourAlliesTerrenceSeul, MSGBOX_DEFAULT
	goto HeartSoul_RetourAllies_Fin
HeartSoul_RetourAllies_TerrenceEtKess:
	msgbox HeartSoul_Text_RetourAlliesTerrenceEtKess, MSGBOX_DEFAULT
	goto HeartSoul_RetourAllies_Fin
HeartSoul_RetourAllies_KessSeule:
	msgbox HeartSoul_Text_RetourAlliesKessSeule, MSGBOX_DEFAULT
HeartSoul_RetourAllies_Fin:
	// histoire.md section 8 (Fuchsia City) : ligne additionnelle selon l'issue du debat du clan
	// (HeartSoul_EventScript_FuchsiaDebatClan, heart_and_soul_act4.inc) - complete la scene
	// plutot que de multiplier les branches ci-dessus (4 x 2 = 8 combinaisons aurait ete trop
	// pour un seul beat de texte).
	goto_if_eq VAR_FUCHSIA_DEBAT, 1, HeartSoul_RetourAllies_ClanDirect
	goto_if_eq VAR_FUCHSIA_DEBAT, 2, HeartSoul_RetourAllies_ClanOmbre
	goto HeartSoul_RetourAllies_ClanFin
HeartSoul_RetourAllies_ClanDirect:
	msgbox HeartSoul_Text_RetourAlliesClanDirect, MSGBOX_DEFAULT
	goto HeartSoul_RetourAllies_ClanFin
HeartSoul_RetourAllies_ClanOmbre:
	msgbox HeartSoul_Text_RetourAlliesClanOmbre, MSGBOX_DEFAULT
HeartSoul_RetourAllies_ClanFin:
	setflag FLAG_RETOUR_ALLIES_VU
HeartSoul_BlueConfrontation_ApresRetourAllies:
	setvar VAR_DIALOGUE_BLUE, 0
	msgbox HeartSoul_Text_BlueRevelation, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_BlueQ1, MSGBOX_DEFAULT
HeartSoul_BlueQ1Choice:
	dynmultipush HeartSoul_Text_BlueQ1_A, 0
	dynmultipush HeartSoul_Text_BlueQ1_B, 1
	dynmultipush HeartSoul_Text_BlueQ1_C, 2
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, HeartSoul_BlueQ1_A
	case 1, HeartSoul_BlueQ1_B
	case 2, HeartSoul_BlueQ1_C
	goto HeartSoul_BlueQ1Choice

HeartSoul_BlueQ1_A:
	addvar VAR_DIALOGUE_BLUE, 4
	msgbox HeartSoul_Text_BlueQ1_A_Resultat, MSGBOX_DEFAULT
	goto HeartSoul_BlueQ2
HeartSoul_BlueQ1_B:
	addvar VAR_DIALOGUE_BLUE, 2
	msgbox HeartSoul_Text_BlueQ1_B_Resultat, MSGBOX_DEFAULT
	goto HeartSoul_BlueQ2
HeartSoul_BlueQ1_C:
	msgbox HeartSoul_Text_BlueQ1_C_Resultat, MSGBOX_DEFAULT
	goto HeartSoul_BlueQ2

HeartSoul_BlueQ2:
	msgbox HeartSoul_Text_BlueQ2, MSGBOX_DEFAULT
HeartSoul_BlueQ2Choice:
	dynmultipush HeartSoul_Text_BlueQ2_A, 0
	dynmultipush HeartSoul_Text_BlueQ2_B, 1
	dynmultipush HeartSoul_Text_BlueQ2_C, 2
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, HeartSoul_BlueQ2_A
	case 1, HeartSoul_BlueQ2_B
	case 2, HeartSoul_BlueQ2_C
	goto HeartSoul_BlueQ2Choice

HeartSoul_BlueQ2_A:
	addvar VAR_DIALOGUE_BLUE, 3
	msgbox HeartSoul_Text_BlueQ2_A_Resultat, MSGBOX_DEFAULT
	goto HeartSoul_BlueQ3
HeartSoul_BlueQ2_B:
	addvar VAR_DIALOGUE_BLUE, 3
	msgbox HeartSoul_Text_BlueQ2_B_Resultat, MSGBOX_DEFAULT
	goto HeartSoul_BlueQ3
HeartSoul_BlueQ2_C:
	addvar VAR_DIALOGUE_BLUE, 1
	msgbox HeartSoul_Text_BlueQ2_C_Resultat, MSGBOX_DEFAULT
	goto HeartSoul_BlueQ3

HeartSoul_BlueQ3:
	msgbox HeartSoul_Text_BlueQ3, MSGBOX_DEFAULT
HeartSoul_BlueQ3Choice:
	dynmultipush HeartSoul_Text_BlueQ3_A, 0
	dynmultipush HeartSoul_Text_BlueQ3_B, 1
	dynmultipush HeartSoul_Text_BlueQ3_C, 2
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, HeartSoul_BlueQ3_A
	case 1, HeartSoul_BlueQ3_B
	case 2, HeartSoul_BlueQ3_C
	goto HeartSoul_BlueQ3Choice

HeartSoul_BlueQ3_A:
	addvar VAR_DIALOGUE_BLUE, 3
	msgbox HeartSoul_Text_BlueQ3_A_Resultat, MSGBOX_DEFAULT
	goto HeartSoul_BlueResoudre
HeartSoul_BlueQ3_B:
	addvar VAR_DIALOGUE_BLUE, 2
	msgbox HeartSoul_Text_BlueQ3_B_Resultat, MSGBOX_DEFAULT
	goto HeartSoul_BlueResoudre
HeartSoul_BlueQ3_C:
	addvar VAR_DIALOGUE_BLUE, 1
	msgbox HeartSoul_Text_BlueQ3_C_Resultat, MSGBOX_DEFAULT
	goto HeartSoul_BlueResoudre

HeartSoul_BlueResoudre:
	goto_if_ge VAR_DIALOGUE_BLUE, 9, HeartSoul_BlueResoudre_Positif
	goto_if_ge VAR_DIALOGUE_BLUE, 6, HeartSoul_BlueResoudre_Neutre
	subvar VAR_REPUTATION, 2
	return
HeartSoul_BlueResoudre_Positif:
	addvar VAR_REPUTATION, 2
	return
HeartSoul_BlueResoudre_Neutre:
	return

// ---------------------------------------------------------------------------------------
// Epilogues (declenches apres FLAG_BLUE_VAINCU, palier VAR_REPUTATION - histoire.md section
// 9 : <=8 ternie, 9-16 equilibree, >=17 exemplaire). FLAG_EPILOGUE_JOUE empeche de le
// rejouer a chaque nouvelle visite de Viridian.
// ---------------------------------------------------------------------------------------
HeartSoul_EventScript_DeclencherEpilogue::
	goto_if_set FLAG_EPILOGUE_JOUE, HeartSoul_Epilogue_Skip
	setflag FLAG_EPILOGUE_JOUE
	goto_if_ge VAR_REPUTATION, 17, HeartSoul_EpilogueExemplaire
	goto_if_ge VAR_REPUTATION, 9, HeartSoul_EpilogueEquilibre
	goto HeartSoul_EpilogueTerni

HeartSoul_EpilogueTerni:
	msgbox HeartSoul_Text_EpilogueTerni1, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_EpilogueTerni2, MSGBOX_DEFAULT
	goto_if_set FLAG_BLAINE_SAUVE, HeartSoul_EpilogueTerni_BlaineSauve
	msgbox HeartSoul_Text_EpilogueTerni_BlaineDisparu, MSGBOX_DEFAULT
	goto HeartSoul_EpilogueTerni_Fin
HeartSoul_EpilogueTerni_BlaineSauve:
	msgbox HeartSoul_Text_EpilogueTerni_BlaineSauve, MSGBOX_DEFAULT
HeartSoul_EpilogueTerni_Fin:
	msgbox HeartSoul_Text_EpilogueTerniFin, MSGBOX_DEFAULT
	return

HeartSoul_EpilogueEquilibre:
	msgbox HeartSoul_Text_EpilogueEquilibre1, MSGBOX_DEFAULT
	goto_if_unset FLAG_BLAINE_SAUVE, HeartSoul_EpilogueEquilibre_SkipBlaine
	msgbox HeartSoul_Text_EpilogueEquilibre_Blaine, MSGBOX_DEFAULT
HeartSoul_EpilogueEquilibre_SkipBlaine:
	goto_if_unset FLAG_TERRENCE_CONVAINCU, HeartSoul_EpilogueEquilibre_SkipTerrence
	msgbox HeartSoul_Text_EpilogueEquilibre_Terrence, MSGBOX_DEFAULT
HeartSoul_EpilogueEquilibre_SkipTerrence:
	goto_if_unset FLAG_KESS_CONVAINCUE, HeartSoul_EpilogueEquilibre_SkipKess
	msgbox HeartSoul_Text_EpilogueEquilibre_Kess, MSGBOX_DEFAULT
HeartSoul_EpilogueEquilibre_SkipKess:
	msgbox HeartSoul_Text_EpilogueEquilibreFin, MSGBOX_DEFAULT
	return

HeartSoul_EpilogueExemplaire:
	msgbox HeartSoul_Text_EpilogueExemplaire1, MSGBOX_DEFAULT
	goto_if_unset FLAG_BLAINE_SAUVE, HeartSoul_EpilogueExemplaire_SkipBlaine
	msgbox HeartSoul_Text_EpilogueExemplaire_Blaine, MSGBOX_DEFAULT
HeartSoul_EpilogueExemplaire_SkipBlaine:
	goto_if_unset FLAG_TERRENCE_CONVAINCU, HeartSoul_EpilogueExemplaire_SkipTerrence
	msgbox HeartSoul_Text_EpilogueExemplaire_Terrence, MSGBOX_DEFAULT
HeartSoul_EpilogueExemplaire_SkipTerrence:
	goto_if_unset FLAG_KESS_CONVAINCUE, HeartSoul_EpilogueExemplaire_SkipKess
	msgbox HeartSoul_Text_EpilogueExemplaire_Kess, MSGBOX_DEFAULT
HeartSoul_EpilogueExemplaire_SkipKess:
	msgbox HeartSoul_Text_EpilogueExemplaireFin, MSGBOX_DEFAULT
	setflag FLAG_ZONE_BONUS_DEBLOQUEE
	msgbox HeartSoul_Text_EpilogueZoneBonus, MSGBOX_DEFAULT
	goto_if_ge VAR_ASPIRANTS_BATTUS, 6, HeartSoul_EpilogueExemplaire_CodaAspirants
	return
HeartSoul_EpilogueExemplaire_CodaAspirants:
	msgbox HeartSoul_Text_EpilogueCodaAspirants, MSGBOX_DEFAULT
	return

HeartSoul_Epilogue_Skip:
	return

HeartSoul_Text_RetourAlliesChampionsSeuls:
	.string "Les Champions déjà ralliés se\n"
	.string "postent le long du couloir,\l"
	.string "gardant chaque sortie.\p"
	.string "Aucune trace de TERRENCE ni de\n"
	.string "KESS. Tu devras faire sans eux.$"

HeartSoul_Text_RetourAlliesTerrenceSeul:
	.string "Les Champions déjà ralliés se\n"
	.string "postent le long du couloir.\p"
	.string "TERRENCE est là aussi,\n"
	.string "silencieux, à surveiller l'accès\l"
	.string "au MANOIR.$"

HeartSoul_Text_RetourAlliesKessSeule:
	.string "Les Champions déjà ralliés se\n"
	.string "postent le long du couloir.\p"
	.string "KESS est là aussi, prête à\n"
	.string "couvrir tes arrières si ça\l"
	.string "tourne mal.$"

HeartSoul_Text_RetourAlliesTerrenceEtKess:
	.string "Les Champions déjà ralliés se\n"
	.string "postent le long du couloir.\p"
	.string "TERRENCE et KESS sont là aussi.\n"
	.string "Tout ce que tu as accompli\l"
	.string "jusqu'ici tient dans ce\l"
	.string "couloir.$"

HeartSoul_Text_RetourAlliesClanDirect:
	.string "Quelques ninjas du clan de\n"
	.string "JANINE se postent en renfort\l"
	.string "visible, comme tu les avais\l"
	.string "encouragés à le faire.$"

HeartSoul_Text_RetourAlliesClanOmbre:
	.string "Aucun ninja du clan de JANINE\n"
	.string "en vue - mais un signal discret\l"
	.string "confirme qu'ils surveillent\l"
	.string "déjà chaque issue, comme tu\l"
	.string "l'avais conseillé.$"

HeartSoul_Text_BlueRevelation:
	.string "BLUE: Tu crois que je fais ça\n"
	.string "pour le pouvoir ?\p"
	.string "J'ai battu l'Élite 4 avant mes\n"
	.string "seize ans. Ils m'ont dit non,\l"
	.string "pas assez 'présentable'.\p"
	.string "Toi, on t'a donné un badge et\n"
	.string "une ville. Moi, une porte\l"
	.string "fermée.$"

HeartSoul_Text_BlueQ1:
	.string "…\p"
	.string "Que lui réponds-tu ?$"

HeartSoul_Text_BlueQ1_A:
	.string "Tu as peut-être raison sur\nl'injustice. Mais ça n'explique\npas CINNABAR.$"
HeartSoul_Text_BlueQ1_B:
	.string "Peu importe ta raison, tu as\ndétruit des vies.$"
HeartSoul_Text_BlueQ1_C:
	.string "Le systeme est casse, tu as\nbien fait de le prouver comme\nça.$"

HeartSoul_Text_BlueQ1_A_Resultat:
	.string "BLUE: …\p"
	.string "Peut-être. Ça n'excuse rien,\n"
	.string "tu as raison.$"

HeartSoul_Text_BlueQ1_B_Resultat:
	.string "BLUE: Facile à dire, avec ton\n"
	.string "badge et ta ville intacts.$"

HeartSoul_Text_BlueQ1_C_Resultat:
	.string "BLUE: …Enfin quelqu'un qui\n"
	.string "comprend.$"

HeartSoul_Text_BlueQ2:
	.string "BLUE: Ces archives prouvent\n"
	.string "que la Ligue savait, pour\l"
	.string "GIOVANNI. Elle a laissé faire\l"
	.string "tant que ça restait discret.$"

HeartSoul_Text_BlueQ2_A:
	.string "Alors prouve-le\npubliquement. Pas comme ça.$"
HeartSoul_Text_BlueQ2_B:
	.string "Ça ne te donne pas le droit\nde faire souffrir CINNABAR.$"
HeartSoul_Text_BlueQ2_C:
	.string "Personne ne te croira de\ntoute façon.$"

HeartSoul_Text_BlueQ2_A_Resultat:
	.string "BLUE: …Peut-être. Trop tard\n"
	.string "pour ça, maintenant.$"

HeartSoul_Text_BlueQ2_B_Resultat:
	.string "BLUE: Non. Ça ne me le donne\n"
	.string "pas.$"

HeartSoul_Text_BlueQ2_C_Resultat:
	.string "BLUE: …\p"
	.string "C'est bien pour ça que j'ai\n"
	.string "choisi cette méthode.$"

HeartSoul_Text_BlueQ3:
	.string "…\p"
	.string "Dernier mot, avant le combat ?$"

HeartSoul_Text_BlueQ3_A:
	.string "Rends-toi. On règlera ça\nautrement.$"
HeartSoul_Text_BlueQ3_B:
	.string "Je vais te prouver que la\nforce ne suffit pas.$"
HeartSoul_Text_BlueQ3_C:
	.string "Tu n'es qu'un lache qui\ncherche des excuses.$"

HeartSoul_Text_BlueQ3_A_Resultat:
	.string "BLUE: Non. Pas après tout ça.\n"
	.string "Bats-toi.$"

HeartSoul_Text_BlueQ3_B_Resultat:
	.string "BLUE: On verra bien.$"

HeartSoul_Text_BlueQ3_C_Resultat:
	.string "BLUE: …\p"
	.string "Ces mots, tu vas les regretter.$"

HeartSoul_Text_EpilogueTerni1:
	.string "CINNABAR renaît, mais\n"
	.string "lentement. Les bateaux\l"
	.string "reviennent un à un, sans\l"
	.string "cérémonie.$"

HeartSoul_Text_EpilogueTerni2:
	.string "PIERRE, ONDINE et les autres\n"
	.string "Champions envoient un message\l"
	.string "officiel, signé, distant. Aucun\l"
	.string "d'eux ne fait le déplacement.$"

HeartSoul_Text_EpilogueTerni_BlaineSauve:
	.string "BLAINE reste en retrait,\n"
	.string "marqué par sa captivité. Il\l"
	.string "n'évoque jamais publiquement\l"
	.string "ce qui s'est passé.$"

HeartSoul_Text_EpilogueTerni_BlaineDisparu:
	.string "La disparition de BLAINE reste\n"
	.string "sans réponse, un vide que même\l"
	.string "la victoire sur BLUE ne comble\l"
	.string "pas.$"

HeartSoul_Text_EpilogueTerniFin:
	.string "On a gagné. Personne n'est\n"
	.string "venu le dire.$"

HeartSoul_Text_EpilogueEquilibre1:
	.string "La reconstruction de CINNABAR\n"
	.string "s'organise avec l'aide\l"
	.string "ponctuelle de quelques\l"
	.string "Champions.$"

HeartSoul_Text_EpilogueEquilibre_Blaine:
	.string "BLAINE reprend discrètement sa\n"
	.string "place de mentor.\p"
	.string "Content que tu aies tenu.$"

HeartSoul_Text_EpilogueEquilibre_Terrence:
	.string "TERRENCE réapparaît,\n"
	.string "disponible pour un combat\l"
	.string "amical.$"

HeartSoul_Text_EpilogueEquilibre_Kess:
	.string "KESS réapparaît, disponible\n"
	.string "pour un combat amical.$"

HeartSoul_Text_EpilogueEquilibreFin:
	.string "Ça ne répare pas tout. Mais\n"
	.string "ça repart.$"

HeartSoul_Text_EpilogueExemplaire1:
	.string "CINNABAR accueille une\n"
	.string "cérémonie de reconstruction\l"
	.string "où tous les Champions du\l"
	.string "KANTO sont présents.$"

HeartSoul_Text_EpilogueExemplaire_Blaine:
	.string "BLAINE prend la parole\n"
	.string "publiquement : il raconte sa\l"
	.string "captivité, salue le joueur, et\l"
	.string "remet en question le silence\l"
	.string "passé de la Ligue.$"

HeartSoul_Text_EpilogueExemplaire_Terrence:
	.string "TERRENCE supervise désormais\n"
	.string "la sécurisation du tunnel.$"

HeartSoul_Text_EpilogueExemplaire_Kess:
	.string "KESS obtient la reconnaissance\n"
	.string "légale de la réserve Safari.$"

HeartSoul_Text_EpilogueExemplaireFin:
	.string "Pour une fois, KANTO entier a\n"
	.string "regardé dans la même\l"
	.string "direction.$"

HeartSoul_Text_EpilogueZoneBonus:
	.string "Un accès aux sous-sols du\n"
	.string "POKéMON MANSION est désormais\l"
	.string "ouvert.$"

HeartSoul_Text_EpilogueCodaAspirants:
	.string "La Ligue KANTO annonce la\n"
	.string "création de nouvelles Arènes\l"
	.string "officielles, pour les types\l"
	.string "que personne n'avait pris au\l"
	.string "sérieux.\p"
	.string "Les dresseurs que tu as\n"
	.string "croisés en chemin y sont\l"
	.string "nommés Champions.$"
