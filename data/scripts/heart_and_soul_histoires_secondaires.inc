// Heart & Soul - histoire.md section 8 (histoires secondaires par lieu), les 2 sous-intrigues
// documentees comme non scriptees a l'issue du chantier Acte V (implementation_notes.md,
// "Non fait, documente comme limitation connue" - debat du clan de Fuchsia est traite a part
// dans heart_and_soul_act4.inc/act5.inc, pas ici, car il se replie dans une scene deja
// existante plutot qu'un nouveau placement). Ces deux-ci necessitent chacun un nouveau
// bg_event sign sur une carte existante, meme methode que heart_and_soul_evenements.inc
// (position verifiee par decodage de map.bin, collision nulle sur la case).

// Celadopole, grand magasin (rez-de-chaussee) : "le grand magasin sert de facade de
// financement pour la Team Rocket. Quete d'infiltration pour exposer ce financement avant
// qu'Erika ne s'engage pleinement." Position (10,6) verifiee par decodage de
// data/layouts/CeladonCity_DepartmentStore_1F_hns/map.bin, a distance des 4 object_events et
// du sign "Directory" deja existants. Le flag pose ici est relu dans
// HeartSoul_EventScript_ErikaConvaincue (heart_and_soul_act4.inc) pour ajouter une ligne de
// remerciement si le joueur l'a trouve avant le combat - sinon aucun changement, la scene
// principale d'Erika n'est pas bloquee par ce sign optionnel.
CeladonCity_DepartmentStore_1F_EventScript_FinancementRocket::
	msgbox HeartSoul_Text_FinancementRocket, MSGBOX_SIGN
	setflag FLAG_FINANCEMENT_ROCKET_EXPOSE
	return

HeartSoul_Text_FinancementRocket:
	.string "Un tiroir mal refermé laisse\n"
	.string "dépasser des registres de\l"
	.string "comptes.\p"
	.string "Les chiffres du GRAND MAGASIN\n"
	.string "ne collent pas avec ses ventes\l"
	.string "officielles. L'argent va\l"
	.string "quelque part - et ce n'est pas\l"
	.string "vers la caisse.$"

// Safrania, Silph Co : "avant la confrontation avec Mira Voss, des documents internes trouves
// dans les bureaux humanisent son parcours (...) a decouvrir avant le combat (...), pour que
// sa motivation ne soit pas amenee seulement par exposition orale." Position (22,7) verifiee
// par decodage de data/layouts/SaffronCity_SilphCo_hns/map.bin, proche de Mira Voss (24,7)
// sans chevaucher son object_event ni les 2 signs deja existants. Contenu volontairement
// distinct (ton de dossier RH, pas de dialogue) de HeartSoul_Text_MiraVossRecit
// (heart_and_soul_act4.inc) : complete la scene parlee plutot que la repeter mot pour mot.
SaffronCity_SilphCo_EventScript_DossierMiraVoss::
	msgbox HeartSoul_Text_DossierMiraVoss, MSGBOX_SIGN
	setflag FLAG_DOSSIER_MIRA_VU
	return

HeartSoul_Text_DossierMiraVoss:
	.string "Un dossier de ressources\n"
	.string "humaines, laissé en évidence.\p"
	.string "MIRA VOSS - Ingénieure en chef\n"
	.string "du projet AZUR. Réaffectée sans\l"
	.string "préavis trois semaines avant le\l"
	.string "lancement. Motif inscrit :\l"
	.string "'restructuration'.$"
