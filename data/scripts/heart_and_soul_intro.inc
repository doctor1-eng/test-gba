// Heart & Soul - choix du type et de l'equipe de depart (Acte 0)
// Genere a partir de docs/heart_and_soul/docs/histoire.md section 6.
// Seul le type Feu (template de reference du brief) mene a une equipe reelle pour l'instant ;
// les 17 autres types affichent un message temporaire et renvoient au choix du type.

HeartSoul_EventScript_ChooseType::
	lockall
	dynmultipush HeartSoul_Text_Type_Normal, TYPE_NORMAL
	dynmultipush HeartSoul_Text_Type_Feu, TYPE_FIRE
	dynmultipush HeartSoul_Text_Type_Eau, TYPE_WATER
	dynmultipush HeartSoul_Text_Type_Plante, TYPE_GRASS
	dynmultipush HeartSoul_Text_Type_Electrik, TYPE_ELECTRIC
	dynmultipush HeartSoul_Text_Type_Glace, TYPE_ICE
	dynmultipush HeartSoul_Text_Type_Combat, TYPE_FIGHTING
	dynmultipush HeartSoul_Text_Type_Poison, TYPE_POISON
	dynmultipush HeartSoul_Text_Type_Sol, TYPE_GROUND
	dynmultipush HeartSoul_Text_Type_Vol, TYPE_FLYING
	dynmultipush HeartSoul_Text_Type_Psy, TYPE_PSYCHIC
	dynmultipush HeartSoul_Text_Type_Insecte, TYPE_BUG
	dynmultipush HeartSoul_Text_Type_Roche, TYPE_ROCK
	dynmultipush HeartSoul_Text_Type_Spectre, TYPE_GHOST
	dynmultipush HeartSoul_Text_Type_Dragon, TYPE_DRAGON
	dynmultipush HeartSoul_Text_Type_Tenebres, TYPE_DARK
	dynmultipush HeartSoul_Text_Type_Acier, TYPE_STEEL
	dynmultipush HeartSoul_Text_Type_Fee, TYPE_FAIRY
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_ChooseType
	copyvar VAR_TYPE_CHOISI, VAR_RESULT
	switch VAR_TYPE_CHOISI
	case TYPE_NORMAL, HeartSoul_EventScript_ChooseTeam_Normal
	case TYPE_FIRE, HeartSoul_EventScript_ChooseTeam_Feu
	case TYPE_WATER, HeartSoul_EventScript_ChooseTeam_Eau
	case TYPE_GRASS, HeartSoul_EventScript_ChooseTeam_Plante
	case TYPE_ELECTRIC, HeartSoul_EventScript_ChooseTeam_Electrik
	case TYPE_ICE, HeartSoul_EventScript_ChooseTeam_Glace
	case TYPE_FIGHTING, HeartSoul_EventScript_ChooseTeam_Combat
	case TYPE_POISON, HeartSoul_EventScript_ChooseTeam_Poison
	case TYPE_GROUND, HeartSoul_EventScript_ChooseTeam_Sol
	case TYPE_FLYING, HeartSoul_EventScript_ChooseTeam_Vol
	case TYPE_PSYCHIC, HeartSoul_EventScript_ChooseTeam_Psy
	case TYPE_BUG, HeartSoul_EventScript_ChooseTeam_Insecte
	case TYPE_ROCK, HeartSoul_EventScript_ChooseTeam_Roche
	case TYPE_GHOST, HeartSoul_EventScript_ChooseTeam_Spectre
	case TYPE_DRAGON, HeartSoul_EventScript_ChooseTeam_Dragon
	case TYPE_DARK, HeartSoul_EventScript_ChooseTeam_Tenebres
	case TYPE_STEEL, HeartSoul_EventScript_ChooseTeam_Acier
	case TYPE_FAIRY, HeartSoul_EventScript_ChooseTeam_Fee
	goto HeartSoul_EventScript_ChooseType
	end

HeartSoul_Text_Type_Normal:
	.string "NORMAL$"
HeartSoul_Text_Type_Feu:
	.string "FEU$"
HeartSoul_Text_Type_Eau:
	.string "EAU$"
HeartSoul_Text_Type_Plante:
	.string "PLANTE$"
HeartSoul_Text_Type_Electrik:
	.string "ELECTRIK$"
HeartSoul_Text_Type_Glace:
	.string "GLACE$"
HeartSoul_Text_Type_Combat:
	.string "COMBAT$"
HeartSoul_Text_Type_Poison:
	.string "POISON$"
HeartSoul_Text_Type_Sol:
	.string "SOL$"
HeartSoul_Text_Type_Vol:
	.string "VOL$"
HeartSoul_Text_Type_Psy:
	.string "PSY$"
HeartSoul_Text_Type_Insecte:
	.string "INSECTE$"
HeartSoul_Text_Type_Roche:
	.string "ROCHE$"
HeartSoul_Text_Type_Spectre:
	.string "SPECTRE$"
HeartSoul_Text_Type_Dragon:
	.string "DRAGON$"
HeartSoul_Text_Type_Tenebres:
	.string "TÉNÈBRES$"
HeartSoul_Text_Type_Acier:
	.string "ACIER$"
HeartSoul_Text_Type_Fee:
	.string "FÉE$"

HeartSoul_EventScript_ChooseTeam_Normal::
	msgbox HeartSoul_Text_TeamNotReady_Normal, MSGBOX_DEFAULT
	goto HeartSoul_EventScript_ChooseType
	end

HeartSoul_Text_TeamNotReady_Normal:
	.string "Ce type n'est pas encore disponible\n"
	.string "dans cette version de Heart & Soul.\p"
	.string "Choisis un autre type pour l'instant.$"

HeartSoul_EventScript_PickOne_Feu::
	dynmultipush HeartSoul_Text_Mon_Feu_0, 0
	dynmultipush HeartSoul_Text_Mon_Feu_1, 1
	dynmultipush HeartSoul_Text_Mon_Feu_2, 2
	dynmultipush HeartSoul_Text_Mon_Feu_3, 3
	dynmultipush HeartSoul_Text_Mon_Feu_4, 4
	dynmultipush HeartSoul_Text_Mon_Feu_5, 5
	dynmultipush HeartSoul_Text_Mon_Feu_6, 6
	dynmultipush HeartSoul_Text_Mon_Feu_7, 7
	dynmultipush HeartSoul_Text_Mon_Feu_8, 8
	dynmultipush HeartSoul_Text_Mon_Feu_9, 9
	dynmultipush HeartSoul_Text_Mon_Feu_10, 10
	dynmultipush HeartSoul_Text_Mon_Feu_11, 11
	dynmultipush HeartSoul_Text_Mon_Feu_12, 12
	dynmultipush HeartSoul_Text_Mon_Feu_13, 13
	dynmultipush HeartSoul_Text_Mon_Feu_14, 14
	dynmultipush HeartSoul_Text_Mon_Feu_15, 15
	dynmultipush HeartSoul_Text_Mon_Feu_16, 16
	dynmultipush HeartSoul_Text_Mon_Feu_17, 17
	dynmultipush HeartSoul_Text_Mon_Feu_18, 18
	dynmultipush HeartSoul_Text_Mon_Feu_19, 19
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_PickOne_Feu
	switch VAR_RESULT
	case 0, HeartSoul_PickOne_Feu_Case0
	case 1, HeartSoul_PickOne_Feu_Case1
	case 2, HeartSoul_PickOne_Feu_Case2
	case 3, HeartSoul_PickOne_Feu_Case3
	case 4, HeartSoul_PickOne_Feu_Case4
	case 5, HeartSoul_PickOne_Feu_Case5
	case 6, HeartSoul_PickOne_Feu_Case6
	case 7, HeartSoul_PickOne_Feu_Case7
	case 8, HeartSoul_PickOne_Feu_Case8
	case 9, HeartSoul_PickOne_Feu_Case9
	case 10, HeartSoul_PickOne_Feu_Case10
	case 11, HeartSoul_PickOne_Feu_Case11
	case 12, HeartSoul_PickOne_Feu_Case12
	case 13, HeartSoul_PickOne_Feu_Case13
	case 14, HeartSoul_PickOne_Feu_Case14
	case 15, HeartSoul_PickOne_Feu_Case15
	case 16, HeartSoul_PickOne_Feu_Case16
	case 17, HeartSoul_PickOne_Feu_Case17
	case 18, HeartSoul_PickOne_Feu_Case18
	case 19, HeartSoul_PickOne_Feu_Case19
	goto HeartSoul_EventScript_PickOne_Feu
	end

HeartSoul_PickOne_Feu_Case0:
	setvar VAR_TEMP_SPECIES, SPECIES_CHARIZARD
	return
HeartSoul_PickOne_Feu_Case1:
	setvar VAR_TEMP_SPECIES, SPECIES_ARCANINE
	return
HeartSoul_PickOne_Feu_Case2:
	setvar VAR_TEMP_SPECIES, SPECIES_RAPIDASH
	return
HeartSoul_PickOne_Feu_Case3:
	setvar VAR_TEMP_SPECIES, SPECIES_MAGMORTAR
	return
HeartSoul_PickOne_Feu_Case4:
	setvar VAR_TEMP_SPECIES, SPECIES_FLAREON
	return
HeartSoul_PickOne_Feu_Case5:
	setvar VAR_TEMP_SPECIES, SPECIES_NINETALES
	return
HeartSoul_PickOne_Feu_Case6:
	setvar VAR_TEMP_SPECIES, SPECIES_TYPHLOSION
	return
HeartSoul_PickOne_Feu_Case7:
	setvar VAR_TEMP_SPECIES, SPECIES_TORKOAL
	return
HeartSoul_PickOne_Feu_Case8:
	setvar VAR_TEMP_SPECIES, SPECIES_CAMERUPT
	return
HeartSoul_PickOne_Feu_Case9:
	setvar VAR_TEMP_SPECIES, SPECIES_HOUNDOOM
	return
HeartSoul_PickOne_Feu_Case10:
	setvar VAR_TEMP_SPECIES, SPECIES_INFERNAPE
	return
HeartSoul_PickOne_Feu_Case11:
	setvar VAR_TEMP_SPECIES, SPECIES_BLAZIKEN
	return
HeartSoul_PickOne_Feu_Case12:
	setvar VAR_TEMP_SPECIES, SPECIES_EMBOAR
	return
HeartSoul_PickOne_Feu_Case13:
	setvar VAR_TEMP_SPECIES, SPECIES_DELPHOX
	return
HeartSoul_PickOne_Feu_Case14:
	setvar VAR_TEMP_SPECIES, SPECIES_TALONFLAME
	return
HeartSoul_PickOne_Feu_Case15:
	setvar VAR_TEMP_SPECIES, SPECIES_VOLCARONA
	return
HeartSoul_PickOne_Feu_Case16:
	setvar VAR_TEMP_SPECIES, SPECIES_CHANDELURE
	return
HeartSoul_PickOne_Feu_Case17:
	setvar VAR_TEMP_SPECIES, SPECIES_SIMISEAR
	return
HeartSoul_PickOne_Feu_Case18:
	setvar VAR_TEMP_SPECIES, SPECIES_PYROAR
	return
HeartSoul_PickOne_Feu_Case19:
	setvar VAR_TEMP_SPECIES, SPECIES_MAGCARGO
	return

HeartSoul_Text_Mon_Feu_0:
	.string "CHARIZARD$"
HeartSoul_Text_Mon_Feu_1:
	.string "ARCANINE$"
HeartSoul_Text_Mon_Feu_2:
	.string "RAPIDASH$"
HeartSoul_Text_Mon_Feu_3:
	.string "MAGMORTAR$"
HeartSoul_Text_Mon_Feu_4:
	.string "FLAREON$"
HeartSoul_Text_Mon_Feu_5:
	.string "NINETALES$"
HeartSoul_Text_Mon_Feu_6:
	.string "TYPHLOSION$"
HeartSoul_Text_Mon_Feu_7:
	.string "TORKOAL$"
HeartSoul_Text_Mon_Feu_8:
	.string "CAMERUPT$"
HeartSoul_Text_Mon_Feu_9:
	.string "HOUNDOOM$"
HeartSoul_Text_Mon_Feu_10:
	.string "INFERNAPE$"
HeartSoul_Text_Mon_Feu_11:
	.string "BLAZIKEN$"
HeartSoul_Text_Mon_Feu_12:
	.string "EMBOAR$"
HeartSoul_Text_Mon_Feu_13:
	.string "DELPHOX$"
HeartSoul_Text_Mon_Feu_14:
	.string "TALONFLAME$"
HeartSoul_Text_Mon_Feu_15:
	.string "VOLCARONA$"
HeartSoul_Text_Mon_Feu_16:
	.string "CHANDELURE$"
HeartSoul_Text_Mon_Feu_17:
	.string "SIMISEAR$"
HeartSoul_Text_Mon_Feu_18:
	.string "PYROAR$"
HeartSoul_Text_Mon_Feu_19:
	.string "MAGCARGO$"

HeartSoul_EventScript_ChooseTeam_Feu::
	call HeartSoul_EventScript_PickOne_Feu
	copyvar VAR_0x8004, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Feu_Slot2::
	call HeartSoul_EventScript_PickOne_Feu
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Feu_Slot2
	copyvar VAR_0x8005, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Feu_Slot3::
	call HeartSoul_EventScript_PickOne_Feu
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Feu_Slot3
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Feu_Slot3
	copyvar VAR_0x8006, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Feu_Slot4::
	call HeartSoul_EventScript_PickOne_Feu
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Feu_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Feu_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8006
	goto_if_eq HeartSoul_ChooseTeam_Feu_Slot4
	copyvar VAR_0x8007, VAR_TEMP_SPECIES
	givemon VAR_0x8004, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8005, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8006, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8007, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	releaseall
	goto HeartSoul_EventScript_CinnabarAttack
	end

HeartSoul_EventScript_ChooseTeam_Eau::
	msgbox HeartSoul_Text_TeamNotReady_Eau, MSGBOX_DEFAULT
	goto HeartSoul_EventScript_ChooseType
	end

HeartSoul_Text_TeamNotReady_Eau:
	.string "Ce type n'est pas encore disponible\n"
	.string "dans cette version de Heart & Soul.\p"
	.string "Choisis un autre type pour l'instant.$"

HeartSoul_EventScript_ChooseTeam_Plante::
	msgbox HeartSoul_Text_TeamNotReady_Plante, MSGBOX_DEFAULT
	goto HeartSoul_EventScript_ChooseType
	end

HeartSoul_Text_TeamNotReady_Plante:
	.string "Ce type n'est pas encore disponible\n"
	.string "dans cette version de Heart & Soul.\p"
	.string "Choisis un autre type pour l'instant.$"

HeartSoul_EventScript_ChooseTeam_Electrik::
	msgbox HeartSoul_Text_TeamNotReady_Electrik, MSGBOX_DEFAULT
	goto HeartSoul_EventScript_ChooseType
	end

HeartSoul_Text_TeamNotReady_Electrik:
	.string "Ce type n'est pas encore disponible\n"
	.string "dans cette version de Heart & Soul.\p"
	.string "Choisis un autre type pour l'instant.$"

HeartSoul_EventScript_ChooseTeam_Glace::
	msgbox HeartSoul_Text_TeamNotReady_Glace, MSGBOX_DEFAULT
	goto HeartSoul_EventScript_ChooseType
	end

HeartSoul_Text_TeamNotReady_Glace:
	.string "Ce type n'est pas encore disponible\n"
	.string "dans cette version de Heart & Soul.\p"
	.string "Choisis un autre type pour l'instant.$"

HeartSoul_EventScript_ChooseTeam_Combat::
	msgbox HeartSoul_Text_TeamNotReady_Combat, MSGBOX_DEFAULT
	goto HeartSoul_EventScript_ChooseType
	end

HeartSoul_Text_TeamNotReady_Combat:
	.string "Ce type n'est pas encore disponible\n"
	.string "dans cette version de Heart & Soul.\p"
	.string "Choisis un autre type pour l'instant.$"

HeartSoul_EventScript_ChooseTeam_Poison::
	msgbox HeartSoul_Text_TeamNotReady_Poison, MSGBOX_DEFAULT
	goto HeartSoul_EventScript_ChooseType
	end

HeartSoul_Text_TeamNotReady_Poison:
	.string "Ce type n'est pas encore disponible\n"
	.string "dans cette version de Heart & Soul.\p"
	.string "Choisis un autre type pour l'instant.$"

HeartSoul_EventScript_ChooseTeam_Sol::
	msgbox HeartSoul_Text_TeamNotReady_Sol, MSGBOX_DEFAULT
	goto HeartSoul_EventScript_ChooseType
	end

HeartSoul_Text_TeamNotReady_Sol:
	.string "Ce type n'est pas encore disponible\n"
	.string "dans cette version de Heart & Soul.\p"
	.string "Choisis un autre type pour l'instant.$"

HeartSoul_EventScript_ChooseTeam_Vol::
	msgbox HeartSoul_Text_TeamNotReady_Vol, MSGBOX_DEFAULT
	goto HeartSoul_EventScript_ChooseType
	end

HeartSoul_Text_TeamNotReady_Vol:
	.string "Ce type n'est pas encore disponible\n"
	.string "dans cette version de Heart & Soul.\p"
	.string "Choisis un autre type pour l'instant.$"

HeartSoul_EventScript_ChooseTeam_Psy::
	msgbox HeartSoul_Text_TeamNotReady_Psy, MSGBOX_DEFAULT
	goto HeartSoul_EventScript_ChooseType
	end

HeartSoul_Text_TeamNotReady_Psy:
	.string "Ce type n'est pas encore disponible\n"
	.string "dans cette version de Heart & Soul.\p"
	.string "Choisis un autre type pour l'instant.$"

HeartSoul_EventScript_ChooseTeam_Insecte::
	msgbox HeartSoul_Text_TeamNotReady_Insecte, MSGBOX_DEFAULT
	goto HeartSoul_EventScript_ChooseType
	end

HeartSoul_Text_TeamNotReady_Insecte:
	.string "Ce type n'est pas encore disponible\n"
	.string "dans cette version de Heart & Soul.\p"
	.string "Choisis un autre type pour l'instant.$"

HeartSoul_EventScript_ChooseTeam_Roche::
	msgbox HeartSoul_Text_TeamNotReady_Roche, MSGBOX_DEFAULT
	goto HeartSoul_EventScript_ChooseType
	end

HeartSoul_Text_TeamNotReady_Roche:
	.string "Ce type n'est pas encore disponible\n"
	.string "dans cette version de Heart & Soul.\p"
	.string "Choisis un autre type pour l'instant.$"

HeartSoul_EventScript_ChooseTeam_Spectre::
	msgbox HeartSoul_Text_TeamNotReady_Spectre, MSGBOX_DEFAULT
	goto HeartSoul_EventScript_ChooseType
	end

HeartSoul_Text_TeamNotReady_Spectre:
	.string "Ce type n'est pas encore disponible\n"
	.string "dans cette version de Heart & Soul.\p"
	.string "Choisis un autre type pour l'instant.$"

HeartSoul_EventScript_ChooseTeam_Dragon::
	msgbox HeartSoul_Text_TeamNotReady_Dragon, MSGBOX_DEFAULT
	goto HeartSoul_EventScript_ChooseType
	end

HeartSoul_Text_TeamNotReady_Dragon:
	.string "Ce type n'est pas encore disponible\n"
	.string "dans cette version de Heart & Soul.\p"
	.string "Choisis un autre type pour l'instant.$"

HeartSoul_EventScript_ChooseTeam_Tenebres::
	msgbox HeartSoul_Text_TeamNotReady_Tenebres, MSGBOX_DEFAULT
	goto HeartSoul_EventScript_ChooseType
	end

HeartSoul_Text_TeamNotReady_Tenebres:
	.string "Ce type n'est pas encore disponible\n"
	.string "dans cette version de Heart & Soul.\p"
	.string "Choisis un autre type pour l'instant.$"

HeartSoul_EventScript_ChooseTeam_Acier::
	msgbox HeartSoul_Text_TeamNotReady_Acier, MSGBOX_DEFAULT
	goto HeartSoul_EventScript_ChooseType
	end

HeartSoul_Text_TeamNotReady_Acier:
	.string "Ce type n'est pas encore disponible\n"
	.string "dans cette version de Heart & Soul.\p"
	.string "Choisis un autre type pour l'instant.$"

HeartSoul_EventScript_ChooseTeam_Fee::
	msgbox HeartSoul_Text_TeamNotReady_Fee, MSGBOX_DEFAULT
	goto HeartSoul_EventScript_ChooseType
	end

HeartSoul_Text_TeamNotReady_Fee:
	.string "Ce type n'est pas encore disponible\n"
	.string "dans cette version de Heart & Soul.\p"
	.string "Choisis un autre type pour l'instant.$"

