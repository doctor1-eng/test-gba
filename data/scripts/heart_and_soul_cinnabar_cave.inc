// Heart & Soul - grotte du Mont Cinnabar (MAP_CINNABAR_ISLAND_CAVE_HNS), histoire.md section 8
// ("Les derniers habitants" / Pokemon Mansion). Realise la carte que la 9e passe de l'Acte I
// (voir implementation_notes.md, section "Elements du document delibarement non repris") avait
// consciemment ecartee - reouverte a la demande explicite de l'utilisateur, qui invoque la
// clause de reversibilite deja ecrite a l'epoque ("reversible si le protagoniste souhaite
// explicitement cette carte separee malgre le compromis").
//
// Portee : purement additive, aucune modification de la sequence d'Acte I deja testee et
// confirmee fonctionnelle par l'utilisateur (le choix guider/cacher, ses flags, son texte,
// restent identiques - voir heart_and_soul_act1.inc). Cette carte est une destination
// optionnelle a explorer librement une fois l'Acte I termine, pas un detour force dans la
// choregraphie de fuite. LOCALID_CINNABAR_CAVE_HABITANT1/2 reprennent volontairement les memes
// sprites que LOCALID_CINNABAR_REFUGEE/LOCALID_CINNABAR_REFUGEE_ENFANT (continuite : ce sont
// les memes personnes, si le joueur a choisi de les cacher plutot que de les guider vers le
// bateau).
//
// Carte geometrique reprise telle quelle de CliffEdgeCave_hns (meme convention que les 3
// batiments de Cinnabar copies depuis VermilionCity_House1_hns) : peindre une nouvelle carte
// necessiterait Porymap, non disponible dans cet environnement.

CinnabarIslandCave_Hns_EventScript_Habitant1::
	msgbox HeartSoul_Text_CaveHabitant1, MSGBOX_NPC
	end

HeartSoul_Text_CaveHabitant1:
	.string "Merci d'avoir gardé le\n"
	.string "silence sur cet endroit.\p"
	.string "On ne sait pas encore si\n"
	.string "c'est sûr de ressortir. Alors\l"
	.string "on attend, ici.$"

CinnabarIslandCave_Hns_EventScript_Habitant2::
	msgbox HeartSoul_Text_CaveHabitant2, MSGBOX_NPC
	end

HeartSoul_Text_CaveHabitant2:
	.string "C'est humide, mais c'est\n"
	.string "mieux que dehors.\p"
	.string "Tu reviendras nous dire quand\n"
	.string "ce sera fini ?$"

CinnabarIslandCave_Hns_EventScript_TracesDepart::
	goto_if_set FLAG_REFUGIES_GUIDES, CinnabarIslandCave_Hns_TracesDepart_Guides
	goto_if_set FLAG_REFUGIES_CACHES, CinnabarIslandCave_Hns_TracesDepart_Caches
	msgbox HeartSoul_Text_CaveTraces_Neutre, MSGBOX_SIGN
	end
CinnabarIslandCave_Hns_TracesDepart_Guides:
	msgbox HeartSoul_Text_CaveTraces_Guides, MSGBOX_SIGN
	end
CinnabarIslandCave_Hns_TracesDepart_Caches:
	msgbox HeartSoul_Text_CaveTraces_Caches, MSGBOX_SIGN
	end

HeartSoul_Text_CaveTraces_Neutre:
	.string "Des traces de pas récentes,\n"
	.string "déjà recouvertes de\l"
	.string "poussière.$"

HeartSoul_Text_CaveTraces_Guides:
	.string "Des traces de pas récentes\n"
	.string "mènent vers la sortie, côté\l"
	.string "mer. Personne n'est resté.$"

HeartSoul_Text_CaveTraces_Caches:
	.string "Un feu de camp éteint, des\n"
	.string "couvertures pliées. Quelqu'un\l"
	.string "vit ici depuis peu.$"
