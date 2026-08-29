// Heart & Soul - Acte II : l'exil et la mefiance
// Deux sous-scripts appeles (call/return, pas goto/end) depuis les scripts existants des
// Champions de PEWTER (PierreDoute, dans PewterCity_Gym_hns/scripts.inc, EventScript_Brock)
// et CERULEAN (OndineDoute, dans CeruleanCity_Gym_hns/scripts.inc, EventScript_Misty).
// Le joueur est deja verrouille (lock/faceplayer) par le script appelant : ces sous-scripts
// ne font ni lock ni release, seulement des msgbox, comme HeartSoul_EventScript_PickOne_*.
// Se declenchent une seule fois chacun (garde FLAG_PIERRE_CONVAINCU / FLAG_ONDINE_CONVAINCUE),
// uniquement une fois l'Acte I termine (FLAG_ACTE_1_TERMINE), avant le combat de badge normal
// qui n'est pas modifie par cette passe.

HeartSoul_EventScript_PierreDoute::
	goto_if_unset FLAG_ACTE_1_TERMINE, HeartSoul_PierreDoute_Skip
	goto_if_set FLAG_PIERRE_CONVAINCU, HeartSoul_PierreDoute_Skip
	msgbox HeartSoul_Text_PierreDouteIntro, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_PierreDouteRecit, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_PierreDouteConclusion, MSGBOX_DEFAULT
	setflag FLAG_PIERRE_CONVAINCU
	call_if_set FLAG_ONDINE_CONVAINCUE, HeartSoul_EventScript_ActeIITermine
	return
HeartSoul_PierreDoute_Skip:
	return

HeartSoul_EventScript_OndineDoute::
	goto_if_unset FLAG_ACTE_1_TERMINE, HeartSoul_OndineDoute_Skip
	goto_if_set FLAG_ONDINE_CONVAINCUE, HeartSoul_OndineDoute_Skip
	msgbox HeartSoul_Text_OndineDouteIntro, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_OndineDouteRecit, MSGBOX_DEFAULT
	msgbox HeartSoul_Text_OndineDouteConclusion, MSGBOX_DEFAULT
	setflag FLAG_ONDINE_CONVAINCUE
	call_if_set FLAG_PIERRE_CONVAINCU, HeartSoul_EventScript_ActeIITermine
	return
HeartSoul_OndineDoute_Skip:
	return

HeartSoul_EventScript_ActeIITermine::
	setflag FLAG_ACTE_2_TERMINE
	return

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
