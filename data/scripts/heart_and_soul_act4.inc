// Heart & Soul - Acte IV : la reconquete (docs/heart_and_soul/docs/histoire.md section 2).
// Meme patron que HeartSoul_EventScript_PierreDoute/OndineDoute/MajorBobDoute
// (heart_and_soul_act2.inc) : sous-scripts appeles (call/return) depuis le script du Champion
// existant, avant le trainerbattle, jamais goto/end. Le joueur est deja verrouille par le
// script appelant. Flag de conviction pose UNIQUEMENT dans la branche de victoire de
// l'appelant (meme motif documente pour Pierre/Ondine : poser le flag ici figerait la scene
// des la premiere tentative, meme en cas de defaite au combat qui suit).
//
// Repond a "l'alliance complete des Champions (Erika, Koga, Sabrina, Major Bob)" prevue par
// histoire.md section 2 (Acte IV) - Major Bob deja fait (Acte II/heart_and_soul_act2.inc).
//
// Decision documentee (pas une correction silencieuse) : la Championne d'Arene de Fuchsia
// dans ce fork est JANINE (TRAINER_JANINE_HNS, FuchsiaCity_Gym_hns), pas Koga - histoire.md
// nomme "Koga (Fuchsia)" mais Koga n'existe pas comme personnage jouable/combattable dans ce
// jeu de base (Janine est sa fille et lui succede, canon depuis les jeux Johto). Plutot que
// d'inventer un nouveau PNJ Koga ou de renommer Janine en Koga (les deux iraient a l'encontre
// de la regle "ne pas reinventer/renommer sans raison"), la scene ci-dessous fait dire a
// Janine qu'elle tient l'Arene de son pere - le lien avec "Koga (Fuchsia)" de histoire.md est
// donc honore narrativement sans toucher aux donnees existantes (TRAINER_JANINE_HNS, son
// equipe, ses PNJ eleves) ni en inventer de nouvelles.
HeartSoul_EventScript_ErikaDoute::
	goto_if_unset FLAG_ACTE_2_TERMINE, HeartSoul_ErikaDoute_Skip
	goto_if_set FLAG_ERIKA_CONVAINCUE, HeartSoul_ErikaDoute_Skip
	msgbox HeartSoul_Text_ErikaDouteIntro, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_ErikaDouteRecit, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_ErikaDouteConclusion, MSGBOX_DEFAULT
	return
HeartSoul_ErikaDoute_Skip:
	return

HeartSoul_EventScript_ErikaConvaincue::
	setflag FLAG_ERIKA_CONVAINCUE
	return

HeartSoul_Text_ErikaDouteIntro:
	.string "ERIKA: …Une île qui tombe, un\n"
	.string "réseau qui s'étend sous nos\l"
	.string "pieds. Vous en êtes sûre ?$"

HeartSoul_Text_ErikaDouteRecit:
	.string "ERIKA: Céladopole vit bien, sans\n"
	.string "arène fermée ni panique.\l"
	.string "Pourquoi je changerais quoi que\l"
	.string "ce soit pour une inconnue ?$"

HeartSoul_Text_ErikaDouteConclusion:
	.string "…\p"
	.string "ERIKA: Un combat, alors. Que je\n"
	.string "voie si votre détermination\l"
	.string "vaut mieux que vos mots.$"

HeartSoul_EventScript_SabrinaDoute::
	goto_if_unset FLAG_ACTE_2_TERMINE, HeartSoul_SabrinaDoute_Skip
	goto_if_set FLAG_SABRINA_CONVAINCUE, HeartSoul_SabrinaDoute_Skip
	msgbox HeartSoul_Text_SabrinaDouteIntro, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_SabrinaDouteRecit, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_SabrinaDouteConclusion, MSGBOX_DEFAULT
	return
HeartSoul_SabrinaDoute_Skip:
	return

HeartSoul_EventScript_SabrinaConvaincue::
	setflag FLAG_SABRINA_CONVAINCUE
	return

HeartSoul_Text_SabrinaDouteIntro:
	.string "SABRINA: …Je savais que vous\n"
	.string "viendriez. Je l'ai vu avant\l"
	.string "même votre arrivée à\l"
	.string "Carmin-sur-Mer.$"

HeartSoul_Text_SabrinaDouteRecit:
	.string "SABRINA: Ce que je ne vois pas\n"
	.string "encore, c'est si vous êtes\l"
	.string "assez forte pour ce qui vous\l"
	.string "attend.$"

HeartSoul_Text_SabrinaDouteConclusion:
	.string "…\p"
	.string "SABRINA: Battons-nous. Ça, au\n"
	.string "moins, m'éclairera.$"

HeartSoul_EventScript_JanineDoute::
	goto_if_unset FLAG_ACTE_2_TERMINE, HeartSoul_JanineDoute_Skip
	goto_if_set FLAG_JANINE_CONVAINCUE, HeartSoul_JanineDoute_Skip
	msgbox HeartSoul_Text_JanineDouteIntro, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_JanineDouteRecit, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_JanineDouteConclusion, MSGBOX_DEFAULT
	return
HeartSoul_JanineDoute_Skip:
	return

HeartSoul_EventScript_JanineConvaincue::
	setflag FLAG_JANINE_CONVAINCUE
	return

HeartSoul_Text_JanineDouteIntro:
	.string "JANINE: Fufufu… Mon père, KOGA,\n"
	.string "m'a laissé cette Arène. Il\l"
	.string "disait qu'un vrai ninja juge\l"
	.string "par les actes, jamais par les\l"
	.string "rumeurs.$"

HeartSoul_Text_JanineDouteRecit:
	.string "JANINE: Une Championne qui a\n"
	.string "perdu son île dès la première\l"
	.string "attaque… les rumeurs ne jouent\l"
	.string "pas en votre faveur.$"

HeartSoul_Text_JanineDouteConclusion:
	.string "…\p"
	.string "JANINE: Alors montrez-moi vos\n"
	.string "actes. Un combat suffira.$"

// Retrouvailles avec Blaine (Seafoam Islands). Decision utilisateur (2026-09-02, meme nature
// de conflit que Blue/Blaine a Cinnabar) : ce jeu de base fait deja combattre un BLAINE
// pleinement fonctionnel a l'Arene de Seafoam (SeafoamIslands_Gym_EventScript_Blaine,
// TRAINER_BLAINE_HNS), ce qui contredit "Blaine disparu jusqu'a l'Acte IV/V" de histoire.md.
// Plutot que de verrouiller l'Arene ou d'ignorer la contradiction, Seafoam devient LE lieu de
// retrouvailles avec Blaine - remplace la mention "Silph Co" de histoire.md section 2 (Acte
// IV), qui n'a de toute facon aucun contenu scripte a ce jour. Le combat de badge existant
// est conserve tel quel (texte original du jeu de base non touche, meme convention que
// Pierre/Ondine/Erika/Sabrina/Janine) : sert desormais de "preuve" que le joueur est pret,
// plutot que d'un simple combat d'Arene anonyme.
HeartSoul_EventScript_BlaineRetrouve::
	goto_if_set FLAG_BLAINE_SAUVE, HeartSoul_BlaineRetrouve_Skip
	msgbox HeartSoul_Text_BlaineRetrouveIntro, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_BlaineRetrouveRecit, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_BlaineRetrouveConclusion, MSGBOX_DEFAULT
	return
HeartSoul_BlaineRetrouve_Skip:
	return

HeartSoul_EventScript_BlaineSauve::
	setflag FLAG_BLAINE_SAUVE
	return

HeartSoul_Text_BlaineRetrouveIntro:
	.string "BLAINE: …{PLAYER} ?\p"
	.string "C'est vraiment toi ? Je pensais\n"
	.string "ne plus jamais revoir personne\l"
	.string "de CINNABAR.$"

HeartSoul_Text_BlaineRetrouveRecit:
	.string "BLAINE: Ils m'ont traîné ici\n"
	.string "après l'attaque. Pas vraiment\l"
	.string "libre, juste assez loin pour\l"
	.string "que personne ne pense à me\l"
	.string "chercher à cet endroit.$"

HeartSoul_Text_BlaineRetrouveConclusion:
	.string "…\p"
	.string "BLAINE: Avant qu'on reparte\n"
	.string "d'ici : montre-moi ce que tu\l"
	.string "vaux. J'ai besoin de savoir que\l"
	.string "CINNABAR a toujours sa\l"
	.string "Championne.$"

HeartSoul_Text_BlaineApresRetrouvailles:
	.string "BLAINE: On rentre à CINNABAR\n"
	.string "dès que possible. Ensemble,\l"
	.string "cette fois.$"

// Kess (Zone Safari / FuchsiaCity_SafariZoneCave_hns), 2e lieutenante avec mecanique de
// conviction (histoire.md section 3, avec Terrence). Meme methode de placement que Lyre/
// Selen/Terrence : aucun PNJ dresseur existant sur cette carte (seulement des Pokemon
// sauvages), nouvel object_event, sprite OBJ_EVENT_GFX_ROCKET_F_HNS (coherence visuelle),
// position (24,12) verifiee par decodage de
// data/layouts/FuchsiaCity_SafariZoneCave_hns/map.bin (collision nulle sur la case et ses 8
// voisines). TRAINER_TYPE_NONE (interaction), meme raison que Terrence : la sequence de
// questions doit precedeer tout combat.
//
// Meme decalage de score que Terrence (voir heart_and_soul_act3.inc pour l'explication
// complete du piege des entiers non signes) : chaque question decalee de +1, seuils
// recalcules >=8/6-7/<=5. Meme court-circuit combat complet sur le choix C agressif de la
// question 1 (histoire.md : "chaque lieutenant retournable", meme regle que Terrence meme
// si non repetee explicitement pour Kess - coherence de mecanique entre les deux seuls
// lieutenants convertibles).
FuchsiaCity_SafariZoneCave_EventScript_Kess::
	lock
	faceplayer
	goto_if_set FLAG_KESS_CONVAINCUE, FuchsiaCity_SafariZoneCave_Kess_ApresConvaincue
	goto_if_set FLAG_KESS_VAINCUE, FuchsiaCity_SafariZoneCave_Kess_ApresVaincue
	goto_if_ne VAR_PERSUASION_KESS, 0, FuchsiaCity_SafariZoneCave_Kess_Evaluate
	msgbox HeartSoul_Text_KessIntro, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_KessQ1, MSGBOX_DEFAULT
FuchsiaCity_SafariZoneCave_Kess_Q1Choice:
	dynmultipush HeartSoul_Text_KessQ1_A, 0
	dynmultipush HeartSoul_Text_KessQ1_B, 1
	dynmultipush HeartSoul_Text_KessQ1_C, 2
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, FuchsiaCity_SafariZoneCave_Kess_Q1_A
	case 1, FuchsiaCity_SafariZoneCave_Kess_Q1_B
	case 2, FuchsiaCity_SafariZoneCave_Kess_Q1_C
	goto FuchsiaCity_SafariZoneCave_Kess_Q1Choice

FuchsiaCity_SafariZoneCave_Kess_Q1_A:
	addvar VAR_PERSUASION_KESS, 3
	msgbox HeartSoul_Text_KessQ1_A_Resultat, MSGBOX_DEFAULT
	goto FuchsiaCity_SafariZoneCave_Kess_Q2
FuchsiaCity_SafariZoneCave_Kess_Q1_B:
	addvar VAR_PERSUASION_KESS, 2
	msgbox HeartSoul_Text_KessQ1_B_Resultat, MSGBOX_DEFAULT
	goto FuchsiaCity_SafariZoneCave_Kess_Q2
FuchsiaCity_SafariZoneCave_Kess_Q1_C:
	setvar VAR_PERSUASION_KESS, 1
	msgbox HeartSoul_Text_KessQ1_C_Resultat, MSGBOX_DEFAULT
	goto FuchsiaCity_SafariZoneCave_Kess_Evaluate

FuchsiaCity_SafariZoneCave_Kess_Q2:
	msgbox HeartSoul_Text_KessQ2, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_KessQ2_Reponse, MSGBOX_DEFAULT
FuchsiaCity_SafariZoneCave_Kess_Q2Choice:
	dynmultipush HeartSoul_Text_KessQ2_A, 0
	dynmultipush HeartSoul_Text_KessQ2_B, 1
	dynmultipush HeartSoul_Text_KessQ2_C, 2
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, FuchsiaCity_SafariZoneCave_Kess_Q2_A
	case 1, FuchsiaCity_SafariZoneCave_Kess_Q2_B
	case 2, FuchsiaCity_SafariZoneCave_Kess_Q2_C
	goto FuchsiaCity_SafariZoneCave_Kess_Q2Choice

FuchsiaCity_SafariZoneCave_Kess_Q2_A:
	addvar VAR_PERSUASION_KESS, 3
	msgbox HeartSoul_Text_KessQ2_A_Resultat, MSGBOX_DEFAULT
	goto FuchsiaCity_SafariZoneCave_Kess_Q3
FuchsiaCity_SafariZoneCave_Kess_Q2_B:
	msgbox HeartSoul_Text_KessQ2_B_Resultat, MSGBOX_DEFAULT
	goto FuchsiaCity_SafariZoneCave_Kess_Q3
FuchsiaCity_SafariZoneCave_Kess_Q2_C:
	addvar VAR_PERSUASION_KESS, 1
	msgbox HeartSoul_Text_KessQ2_C_Resultat, MSGBOX_DEFAULT
	goto FuchsiaCity_SafariZoneCave_Kess_Q3

FuchsiaCity_SafariZoneCave_Kess_Q3:
	msgbox HeartSoul_Text_KessQ3, MSGBOX_DEFAULT
FuchsiaCity_SafariZoneCave_Kess_Q3Choice:
	dynmultipush HeartSoul_Text_KessQ3_A, 0
	dynmultipush HeartSoul_Text_KessQ3_B, 1
	dynmultipush HeartSoul_Text_KessQ3_C, 2
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	switch VAR_RESULT
	case 0, FuchsiaCity_SafariZoneCave_Kess_Q3_A
	case 1, FuchsiaCity_SafariZoneCave_Kess_Q3_B
	case 2, FuchsiaCity_SafariZoneCave_Kess_Q3_C
	goto FuchsiaCity_SafariZoneCave_Kess_Q3Choice

FuchsiaCity_SafariZoneCave_Kess_Q3_A:
	addvar VAR_PERSUASION_KESS, 2
	msgbox HeartSoul_Text_KessQ3_A_Resultat, MSGBOX_DEFAULT
	goto FuchsiaCity_SafariZoneCave_Kess_Evaluate
FuchsiaCity_SafariZoneCave_Kess_Q3_B:
	addvar VAR_PERSUASION_KESS, 3
	msgbox HeartSoul_Text_KessQ3_B_Resultat, MSGBOX_DEFAULT
	goto FuchsiaCity_SafariZoneCave_Kess_Evaluate
FuchsiaCity_SafariZoneCave_Kess_Q3_C:
	addvar VAR_PERSUASION_KESS, 1
	msgbox HeartSoul_Text_KessQ3_C_Resultat, MSGBOX_DEFAULT
	goto FuchsiaCity_SafariZoneCave_Kess_Evaluate

FuchsiaCity_SafariZoneCave_Kess_Evaluate:
	goto_if_ge VAR_PERSUASION_KESS, 8, FuchsiaCity_SafariZoneCave_Kess_Ralliee
	goto_if_ge VAR_PERSUASION_KESS, 6, FuchsiaCity_SafariZoneCave_Kess_CombatAllege
	goto FuchsiaCity_SafariZoneCave_Kess_CombatComplet

FuchsiaCity_SafariZoneCave_Kess_Ralliee:
	msgbox HeartSoul_Text_KessRalliee, MSGBOX_DEFAULT
	setflag FLAG_KESS_CONVAINCUE
	setflag FLAG_KESS_VAINCUE
	addvar VAR_REPUTATION, 2
	closemessage
	release
	end

FuchsiaCity_SafariZoneCave_Kess_CombatAllege:
	msgbox HeartSoul_Text_KessCombatAllege, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_KESS_LIGHT_HNS, HeartSoul_Text_KessLightSeen, HeartSoul_Text_KessLightBeaten, FuchsiaCity_SafariZoneCave_EventScript_KessAllegeVictoire
	msgbox HeartSoul_Text_KessLightAfterBattle, MSGBOX_DEFAULT
	closemessage
	release
	end

FuchsiaCity_SafariZoneCave_EventScript_KessAllegeVictoire::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox HeartSoul_Text_KessLightAfterBattle, MSGBOX_DEFAULT
	setflag FLAG_KESS_CONVAINCUE
	setflag FLAG_KESS_VAINCUE
	addvar VAR_REPUTATION, 1
	closemessage
	release
	end

FuchsiaCity_SafariZoneCave_Kess_CombatComplet:
	msgbox HeartSoul_Text_KessCombatComplet, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_KESS_HNS, HeartSoul_Text_KessSeen, HeartSoul_Text_KessBeaten, FuchsiaCity_SafariZoneCave_EventScript_KessVictoire
	msgbox HeartSoul_Text_KessAfterBattle, MSGBOX_DEFAULT
	closemessage
	release
	end

FuchsiaCity_SafariZoneCave_EventScript_KessVictoire::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox HeartSoul_Text_KessAfterBattle, MSGBOX_DEFAULT
	setflag FLAG_KESS_VAINCUE
	closemessage
	release
	end

FuchsiaCity_SafariZoneCave_Kess_ApresConvaincue:
	msgbox HeartSoul_Text_KessApresConvaincue, MSGBOX_DEFAULT
	closemessage
	release
	end

FuchsiaCity_SafariZoneCave_Kess_ApresVaincue:
	msgbox HeartSoul_Text_KessAfterBattle, MSGBOX_DEFAULT
	closemessage
	release
	end

HeartSoul_Text_KessIntro:
	.string "KESS: …Ne m'oblige pas à faire\n"
	.string "ça.$"

HeartSoul_Text_KessQ1:
	.string "KESS: BLUE menace vraiment mes\n"
	.string "employés si je n'obéis pas.\l"
	.string "Tu veux savoir si c'est\l"
	.string "sérieux ?$"

HeartSoul_Text_KessQ1_A:
	.string "Je peux organiser leur\nprotection des maintenant$"
HeartSoul_Text_KessQ1_B:
	.string "C'est vraiment grave ?$"
HeartSoul_Text_KessQ1_C:
	.string "Ce n'est pas mon probleme,\nbats-toi$"

HeartSoul_Text_KessQ1_A_Resultat:
	.string "KESS: …Tu ferais ça ? Pour de\n"
	.string "vrai ?$"

HeartSoul_Text_KessQ1_B_Resultat:
	.string "KESS: Assez pour que je ne\n"
	.string "dorme plus la nuit.$"

HeartSoul_Text_KessQ1_C_Resultat:
	.string "KESS: …\p"
	.string "Alors on n'a plus rien à se\n"
	.string "dire.$"

HeartSoul_Text_KessQ2:
	.string "KESS: Le braconnage que je fais\n"
	.string "pour BLUE… tu crois que j'y\l"
	.string "crois ?\p"
	.string "Non. Mais je n'ai pas le choix.$"

HeartSoul_Text_KessQ2_Reponse:
	.string "…\p"
	.string "Alors laisse-moi te donner ce\n"
	.string "choix.$"

HeartSoul_Text_KessQ2_A:
	.string "(Le dire sincerement)$"
HeartSoul_Text_KessQ2_B:
	.string "Menacer de reveler son role\npubliquement$"
HeartSoul_Text_KessQ2_C:
	.string "Ne rien dire de concret$"

HeartSoul_Text_KessQ2_A_Resultat:
	.string "KESS: …\p"
	.string "Personne ne me l'avait\n"
	.string "proposé aussi simplement.$"

HeartSoul_Text_KessQ2_B_Resultat:
	.string "KESS: Fais ça, et je perds tout\n"
	.string "avant même que BLUE ne perde.\l"
	.string "Recule.$"

HeartSoul_Text_KessQ2_C_Resultat:
	.string "KESS: …\p"
	.string "Ouais. C'est bien ce que je\n"
	.string "pensais.$"

HeartSoul_Text_KessQ3:
	.string "KESS: Si je bats… si tu bats\n"
	.string "BLUE, la réserve retrouve son\l"
	.string "statut légal. Tu as ma parole.$"

HeartSoul_Text_KessQ3_A:
	.string "Accepter la promesse$"
HeartSoul_Text_KessQ3_B:
	.string "Exiger une garantie ecrite\nd'un Champion$"
HeartSoul_Text_KessQ3_C:
	.string "Ignorer, combattre directement$"

HeartSoul_Text_KessQ3_A_Resultat:
	.string "KESS: …\p"
	.string "D'accord. Je te fais confiance.$"

HeartSoul_Text_KessQ3_B_Resultat:
	.string "KESS: Une garantie écrite…\n"
	.string "Oui. Ça, je peux y croire.$"

HeartSoul_Text_KessQ3_C_Resultat:
	.string "KESS: …\p"
	.string "Comme tu veux.$"

HeartSoul_Text_KessRalliee:
	.string "KESS: …Merci. Je vais faire\n"
	.string "libérer les Pokémon retenus\l"
	.string "ici.$"

HeartSoul_Text_KessCombatAllege:
	.string "KESS: …T'as raison sur un\n"
	.string "point. Mais je dois quand même\l"
	.string "vérifier que tu es à la\l"
	.string "hauteur.$"

HeartSoul_Text_KessLightSeen:
	.string "KESS: Bats-toi.$"

HeartSoul_Text_KessLightBeaten:
	.string "KESS: …\p"
	.string "Merci.$"

HeartSoul_Text_KessLightAfterBattle:
	.string "KESS: La Zone Safari va\n"
	.string "changer. Je m'en occupe.$"

HeartSoul_Text_KessCombatComplet:
	.string "KESS: Je n'ai pas le choix.\n"
	.string "Désolée.$"

HeartSoul_Text_KessSeen:
	.string "KESS: Désolée.$"

HeartSoul_Text_KessBeaten:
	.string "KESS: …\p"
	.string "C'est terminé.$"

HeartSoul_Text_KessAfterBattle:
	.string "KESS: La Zone Safari est de\n"
	.string "nouveau accessible.$"

HeartSoul_Text_KessApresConvaincue:
	.string "KESS: Les Pokémon de cette\n"
	.string "réserve te remercient, même\l"
	.string "s'ils ne le savent pas.$"

// Mira Voss (Silph Co / SaffronCity_SilphCo_hns), dernier lieutenant (histoire.md section 3 :
// "la plus dangereuse et la plus lucide des cinq", combat direct, pas de mecanique de
// conviction - seuls Terrence et Kess sont convertibles). Meme methode de placement que les
// 3 precedents object_events crees pour cette raison : aucun dresseur existant sur cette
// carte (Officer/Steven/Receptionist sont tous des PNJ narratifs sans combat), sprite
// OBJ_EVENT_GFX_ROCKET_F_HNS, position (24,7) verifiee par decodage de
// data/layouts/SaffronCity_SilphCo_hns/map.bin (collision nulle sur la case et ses 8
// voisines, a distance des 3 PNJ existants).
//
// Defaite de Mira Voss = dernier verrou avant l'Acte V (voir
// HeartSoul_EventScript_VerifierDeblocageActeV plus bas, meme fichier).
SaffronCity_SilphCo_EventScript_MiraVoss::
	lock
	faceplayer
	goto_if_set FLAG_MIRA_VOSS_VAINCUE, SaffronCity_SilphCo_MiraVoss_ApresVaincue
	msgbox HeartSoul_Text_MiraVossIntro, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_MiraVossRecit, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_MIRA_VOSS_HNS, HeartSoul_Text_MiraVossSeen, HeartSoul_Text_MiraVossBeaten, SaffronCity_SilphCo_EventScript_MiraVossVictoire
	msgbox HeartSoul_Text_MiraVossAfterBattle, MSGBOX_DEFAULT
	closemessage
	release
	end

SaffronCity_SilphCo_EventScript_MiraVossVictoire::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox HeartSoul_Text_MiraVossAfterBattle, MSGBOX_DEFAULT
	setflag FLAG_MIRA_VOSS_VAINCUE
	call HeartSoul_EventScript_VerifierDeblocageActeV
	closemessage
	release
	end

SaffronCity_SilphCo_MiraVoss_ApresVaincue:
	msgbox HeartSoul_Text_MiraVossApresVaincue, MSGBOX_DEFAULT
	closemessage
	release
	end

HeartSoul_Text_MiraVossIntro:
	.string "MIRA VOSS: J'ai dirigé un\n"
	.string "projet entier pour SILPH CO.\l"
	.string "On m'a écartée à la dernière\l"
	.string "minute, un cadre mieux\l"
	.string "introduit a pris ma place.$"

HeartSoul_Text_MiraVossRecit:
	.string "MIRA VOSS: BLUE, on lui a fait\n"
	.string "la même chose, en pire. Alors\l"
	.string "oui, je le suis. Sans lui, je\l"
	.string "ne serais qu'une ingénieure\l"
	.string "de plus qu'on a oubliée.$"

HeartSoul_Text_MiraVossSeen:
	.string "MIRA VOSS: Voyons ce que\n"
	.string "vaut vraiment ta détermination.$"

HeartSoul_Text_MiraVossBeaten:
	.string "MIRA VOSS: …\p"
	.string "Impressionnant. Vraiment.$"

HeartSoul_Text_MiraVossAfterBattle:
	.string "MIRA VOSS: SILPH CO est à toi.\n"
	.string "BLUE devra se débrouiller sans\l"
	.string "moi, pour la suite.$"

HeartSoul_Text_MiraVossApresVaincue:
	.string "MIRA VOSS: Je reste ici. J'ai\n"
	.string "encore des dossiers à trier.$"
