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
