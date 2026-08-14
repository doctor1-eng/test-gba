@ Kanto Saison 1 : maison presque vide, volets fermés depuis l'extérieur — laissée intentionnellement
@ mystérieuse comme accroche pour un futur arc narratif (cf. CHANGELOG). Pas de PNJ pour l'instant.
LittlerootTown_MaisonVoletsFermes_MapScripts::
	.byte 0

LittlerootTown_MaisonVoletsFermes_EventScript_Note::
	msgbox LittlerootTown_MaisonVoletsFermes_Text_Note, MSGBOX_SIGN
	end

LittlerootTown_MaisonVoletsFermes_Text_Note:
	.string "Un vieux mot laissé sur la table,\n"
	.string "à moitié effacé par la poussière…\p"
	.string "“…reviendrai bientôt. Ne laisse\n"
	.string "personne entrer avant que…”\p"
	.string "Le reste est illisible.$"
