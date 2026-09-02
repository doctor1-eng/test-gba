// Heart & Soul - contenu interne des 3 batiments de Cinnabar (Gym/Manoir/Labo).
// Chantier "meubler les blockouts" : les 3 pieces sont des blockouts identiques et vides
// depuis leur reconstruction en maps natives _Hns (voir docs/heart_and_soul/technical_map.md,
// section "Batiments de Cinnabar"). Pas de nouvelle tuile ajoutee (regle "ne pas inventer les
// tuiles") : uniquement des object_events (sprites deja compiles OBJ_EVENT_GFX_WORKER_M_HNS /
// OBJ_EVENT_GFX_SCIENTIST_M_HNS, deja utilises ailleurs dans le jeu) et des bg_events de type
// "sign" (meme mecanisme que Route1_EventScript_Sign, aucun sprite - juste un texte lu sur
// place). Positions verifiees par decodage direct de map.bin (les 3 batiments partagent
// exactement le meme layout 13x10, copie de VermilionCity_House1_hns) : zone ouverte
// colonnes 0-10 / lignes 2-7, sauf un obstacle 2x2 en (5-6,4-5) - toutes les positions
// utilisees ci-dessous evitent cet obstacle.
//
// Portee volontairement limitee (pas de sur-correction) : un PNJ par piece pour l'Arene et
// le Labo (texte seul, pas de combat, pas de nouveau flag), 2 panneaux pour le Manoir
// reprenant le fil narratif deja specifie dans histoire.md section 8 ("les carnets de
// Blaine"). Aucune nouvelle mecanique, aucun nouveau flag/var necessaire.

// Arene : resout la contradiction deja documentee dans MAP_TEST_README.md ("le panneau
// existant devant l'Arene contredit maintenant la porte fonctionnelle juste a cote") en
// donnant une raison diegetique a la piece vide : reconstruction en cours, Blaine toujours
// au Dojo (coherent avec CinnabarIsland_EventScript_Blaine, deja existant).
CinnabarIslandGym_Hns_EventScript_Worker::
	msgbox HeartSoul_Text_GymWorker, MSGBOX_NPC
	end

HeartSoul_Text_GymWorker:
	.string "OUVRIER: On remet l'ARENE en\n"
	.string "etat, mur par mur. Ca prend le\l"
	.string "temps que ca prend.\p"
	.string "BLAINE dirige les combats\n"
	.string "depuis le DOJO en attendant.\l"
	.string "Revenez quand ce sera fini.$"

// Labo : coherent avec l'etat "Acte I - Blaine disparu" (FLAG_BLAINE_DISPARU) deja pose des
// l'attaque de Cinnabar - le labo tourne au ralenti, pas a l'arret total (une assistante
// reste), en echo au theme "la confiance se reconquiert" deja utilise ailleurs (Pierre,
// Ondine).
CinnabarIslandPokemonLab_Hns_EventScript_Assistant::
	msgbox HeartSoul_Text_LabAssistant, MSGBOX_NPC
	end

HeartSoul_Text_LabAssistant:
	.string "ASSISTANTE: Le LABO tourne au\n"
	.string "ralenti depuis que BLAINE a\l"
	.string "disparu.\p"
	.string "Personne n'a eu le cœur a\n"
	.string "continuer ses recherches sans\l"
	.string "lui. On garde les lieux, c'est\l"
	.string "deja ca.$"

// Manoir : 2 fragments des "carnets de Blaine" (histoire.md section 8), fil qui se referme en
// Acte V ("c'est ce que BLUE cherchait precisement a recuperer"). Contenu ecrit pour
// preparer cette revelation sans la devoiler completement ici - reste volontairement
// elliptique (rien sur GIOVANNI ou BLUE nomme explicitement dans le 2e fragment, juste assez
// pour semer le doute).
CinnabarIslandMansion_Hns_EventScript_CarnetBlaine1::
	msgbox HeartSoul_Text_CarnetBlaine1, MSGBOX_SIGN
	end

HeartSoul_Text_CarnetBlaine1:
	.string "Une page arrachee, glissee sous\n"
	.string "une latte du plancher.\p"
	.string "'Les dossiers de GIOVANNI\n"
	.string "parlent d'accélération\l"
	.string "artificielle. Des mots trop\l"
	.string "propres pour ce qu'ils cachent\l"
	.string "vraiment.'$"

CinnabarIslandMansion_Hns_EventScript_CarnetBlaine2::
	msgbox HeartSoul_Text_CarnetBlaine2, MSGBOX_SIGN
	end

HeartSoul_Text_CarnetBlaine2:
	.string "Une autre page, plus recente,\n"
	.string "l'encre a peine seche.\p"
	.string "'Je n'ai jamais tout dit a la\n"
	.string "Ligue. Si quelqu'un met la main\l"
	.string "sur ces notes un jour, il\l"
	.string "comprendra pourquoi je me\l"
	.string "méfie de certaines personnes.'$"
