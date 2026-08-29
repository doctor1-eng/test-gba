// Heart & Soul - choix du type et de l'equipe de depart (Acte 0)
// Genere a partir de docs/heart_and_soul/docs/histoire.md section 6.
// Noms de Pokemon en francais (verifies un par un par recherche web).

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

HeartSoul_EventScript_PickOne_Normal::
	dynmultipush HeartSoul_Text_Mon_Normal_0, 0
	dynmultipush HeartSoul_Text_Mon_Normal_1, 1
	dynmultipush HeartSoul_Text_Mon_Normal_2, 2
	dynmultipush HeartSoul_Text_Mon_Normal_3, 3
	dynmultipush HeartSoul_Text_Mon_Normal_4, 4
	dynmultipush HeartSoul_Text_Mon_Normal_5, 5
	dynmultipush HeartSoul_Text_Mon_Normal_6, 6
	dynmultipush HeartSoul_Text_Mon_Normal_7, 7
	dynmultipush HeartSoul_Text_Mon_Normal_8, 8
	dynmultipush HeartSoul_Text_Mon_Normal_9, 9
	dynmultipush HeartSoul_Text_Mon_Normal_10, 10
	dynmultipush HeartSoul_Text_Mon_Normal_11, 11
	dynmultipush HeartSoul_Text_Mon_Normal_12, 12
	dynmultipush HeartSoul_Text_Mon_Normal_13, 13
	dynmultipush HeartSoul_Text_Mon_Normal_14, 14
	dynmultipush HeartSoul_Text_Mon_Normal_15, 15
	dynmultipush HeartSoul_Text_Mon_Normal_16, 16
	dynmultipush HeartSoul_Text_Mon_Normal_17, 17
	dynmultipush HeartSoul_Text_Mon_Normal_18, 18
	dynmultipush HeartSoul_Text_Mon_Normal_19, 19
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_PickOne_Normal
	switch VAR_RESULT
	case 0, HeartSoul_PickOne_Normal_Case0
	case 1, HeartSoul_PickOne_Normal_Case1
	case 2, HeartSoul_PickOne_Normal_Case2
	case 3, HeartSoul_PickOne_Normal_Case3
	case 4, HeartSoul_PickOne_Normal_Case4
	case 5, HeartSoul_PickOne_Normal_Case5
	case 6, HeartSoul_PickOne_Normal_Case6
	case 7, HeartSoul_PickOne_Normal_Case7
	case 8, HeartSoul_PickOne_Normal_Case8
	case 9, HeartSoul_PickOne_Normal_Case9
	case 10, HeartSoul_PickOne_Normal_Case10
	case 11, HeartSoul_PickOne_Normal_Case11
	case 12, HeartSoul_PickOne_Normal_Case12
	case 13, HeartSoul_PickOne_Normal_Case13
	case 14, HeartSoul_PickOne_Normal_Case14
	case 15, HeartSoul_PickOne_Normal_Case15
	case 16, HeartSoul_PickOne_Normal_Case16
	case 17, HeartSoul_PickOne_Normal_Case17
	case 18, HeartSoul_PickOne_Normal_Case18
	case 19, HeartSoul_PickOne_Normal_Case19
	goto HeartSoul_EventScript_PickOne_Normal
	end

HeartSoul_PickOne_Normal_Case0:
	setvar VAR_TEMP_SPECIES, SPECIES_SNORLAX
	return
HeartSoul_PickOne_Normal_Case1:
	setvar VAR_TEMP_SPECIES, SPECIES_KANGASKHAN
	return
HeartSoul_PickOne_Normal_Case2:
	setvar VAR_TEMP_SPECIES, SPECIES_CHANSEY
	return
HeartSoul_PickOne_Normal_Case3:
	setvar VAR_TEMP_SPECIES, SPECIES_BLISSEY
	return
HeartSoul_PickOne_Normal_Case4:
	setvar VAR_TEMP_SPECIES, SPECIES_TAUROS
	return
HeartSoul_PickOne_Normal_Case5:
	setvar VAR_TEMP_SPECIES, SPECIES_DITTO
	return
HeartSoul_PickOne_Normal_Case6:
	setvar VAR_TEMP_SPECIES, SPECIES_SLAKING
	return
HeartSoul_PickOne_Normal_Case7:
	setvar VAR_TEMP_SPECIES, SPECIES_GIRAFARIG
	return
HeartSoul_PickOne_Normal_Case8:
	setvar VAR_TEMP_SPECIES, SPECIES_FURRET
	return
HeartSoul_PickOne_Normal_Case9:
	setvar VAR_TEMP_SPECIES, SPECIES_LINOONE
	return
HeartSoul_PickOne_Normal_Case10:
	setvar VAR_TEMP_SPECIES, SPECIES_ZANGOOSE
	return
HeartSoul_PickOne_Normal_Case11:
	setvar VAR_TEMP_SPECIES, SPECIES_BIBAREL
	return
HeartSoul_PickOne_Normal_Case12:
	setvar VAR_TEMP_SPECIES, SPECIES_STOUTLAND
	return
HeartSoul_PickOne_Normal_Case13:
	setvar VAR_TEMP_SPECIES, SPECIES_DIGGERSBY
	return
HeartSoul_PickOne_Normal_Case14:
	setvar VAR_TEMP_SPECIES, SPECIES_LOPUNNY
	return
HeartSoul_PickOne_Normal_Case15:
	setvar VAR_TEMP_SPECIES, SPECIES_URSARING
	return
HeartSoul_PickOne_Normal_Case16:
	setvar VAR_TEMP_SPECIES, SPECIES_EXPLOUD
	return
HeartSoul_PickOne_Normal_Case17:
	setvar VAR_TEMP_SPECIES, SPECIES_DODUO
	return
HeartSoul_PickOne_Normal_Case18:
	setvar VAR_TEMP_SPECIES, SPECIES_DODRIO
	return
HeartSoul_PickOne_Normal_Case19:
	setvar VAR_TEMP_SPECIES, SPECIES_PORYGON2
	return

HeartSoul_Text_Mon_Normal_0:
	.string "RONFLEX$"
HeartSoul_Text_Mon_Normal_1:
	.string "KANGOUREX$"
HeartSoul_Text_Mon_Normal_2:
	.string "LEVEINARD$"
HeartSoul_Text_Mon_Normal_3:
	.string "LEUPHORIE$"
HeartSoul_Text_Mon_Normal_4:
	.string "TAUROS$"
HeartSoul_Text_Mon_Normal_5:
	.string "MÉTAMORPH$"
HeartSoul_Text_Mon_Normal_6:
	.string "MONAFLÈMIT$"
HeartSoul_Text_Mon_Normal_7:
	.string "GIRAFARIG$"
HeartSoul_Text_Mon_Normal_8:
	.string "FOUINAR$"
HeartSoul_Text_Mon_Normal_9:
	.string "LINÉON$"
HeartSoul_Text_Mon_Normal_10:
	.string "MANGRIFF$"
HeartSoul_Text_Mon_Normal_11:
	.string "CASTORNO$"
HeartSoul_Text_Mon_Normal_12:
	.string "MASTOUFFE$"
HeartSoul_Text_Mon_Normal_13:
	.string "EXCAVARENNE$"
HeartSoul_Text_Mon_Normal_14:
	.string "LOCKPIN$"
HeartSoul_Text_Mon_Normal_15:
	.string "URSARING$"
HeartSoul_Text_Mon_Normal_16:
	.string "BROUHABAM$"
HeartSoul_Text_Mon_Normal_17:
	.string "DODUO$"
HeartSoul_Text_Mon_Normal_18:
	.string "DODRIO$"
HeartSoul_Text_Mon_Normal_19:
	.string "PORYGON2$"

HeartSoul_EventScript_ChooseTeam_Normal::
	call HeartSoul_EventScript_PickOne_Normal
	copyvar VAR_0x8004, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Normal_Slot2::
	call HeartSoul_EventScript_PickOne_Normal
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Normal_Slot2
	copyvar VAR_0x8005, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Normal_Slot3::
	call HeartSoul_EventScript_PickOne_Normal
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Normal_Slot3
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Normal_Slot3
	copyvar VAR_0x8006, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Normal_Slot4::
	call HeartSoul_EventScript_PickOne_Normal
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Normal_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Normal_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8006
	goto_if_eq HeartSoul_ChooseTeam_Normal_Slot4
	copyvar VAR_0x8007, VAR_TEMP_SPECIES
	givemon VAR_0x8004, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8005, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8006, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8007, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	setflag FLAG_SYS_POKEMON_GET
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox HeartSoul_Text_ChoisisAttaquesIntro, MSGBOX_DEFAULT
HeartSoul_ChooseMoves_Normal_ChooseMon::
	msgbox HeartSoul_Text_ChoisirPokemonAttaque, MSGBOX_DEFAULT
	setmoverelearnerstate MOVE_RELEARNER_LEVEL_UP_MOVES
	chooseboxmon SELECT_PC_MON_MOVE_RELEARNER
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartSoul_EventScript_CinnabarAttack
	special HasMovesToRelearn
	goto_if_eq VAR_RESULT, FALSE, HeartSoul_ChooseMoves_Normal_NoMove
	msgbox HeartSoul_Text_QuelleAttaque, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto HeartSoul_ChooseMoves_Normal_ChooseMon
HeartSoul_ChooseMoves_Normal_NoMove:
	msgbox HeartSoul_Text_PlusRienAApprendre, MSGBOX_DEFAULT
	goto HeartSoul_ChooseMoves_Normal_ChooseMon
	end

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
	.string "DRACAUFEU$"
HeartSoul_Text_Mon_Feu_1:
	.string "ARCANIN$"
HeartSoul_Text_Mon_Feu_2:
	.string "GALOPA$"
HeartSoul_Text_Mon_Feu_3:
	.string "MAGANON$"
HeartSoul_Text_Mon_Feu_4:
	.string "PYROLI$"
HeartSoul_Text_Mon_Feu_5:
	.string "FEUNARD$"
HeartSoul_Text_Mon_Feu_6:
	.string "TYPHLOSION$"
HeartSoul_Text_Mon_Feu_7:
	.string "CHARTOR$"
HeartSoul_Text_Mon_Feu_8:
	.string "CAMÉRUPT$"
HeartSoul_Text_Mon_Feu_9:
	.string "DÉMOLOSSE$"
HeartSoul_Text_Mon_Feu_10:
	.string "SIMIABRAZ$"
HeartSoul_Text_Mon_Feu_11:
	.string "BRASÉGALI$"
HeartSoul_Text_Mon_Feu_12:
	.string "ROITIFLAM$"
HeartSoul_Text_Mon_Feu_13:
	.string "GOUPELIN$"
HeartSoul_Text_Mon_Feu_14:
	.string "FLAMBUSARD$"
HeartSoul_Text_Mon_Feu_15:
	.string "PYRAX$"
HeartSoul_Text_Mon_Feu_16:
	.string "LUGULABRE$"
HeartSoul_Text_Mon_Feu_17:
	.string "FLAMOUTAN$"
HeartSoul_Text_Mon_Feu_18:
	.string "NEMELIOS$"
HeartSoul_Text_Mon_Feu_19:
	.string "VOLCAROPOD$"

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
	setflag FLAG_SYS_POKEMON_GET
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox HeartSoul_Text_ChoisisAttaquesIntro, MSGBOX_DEFAULT
HeartSoul_ChooseMoves_Feu_ChooseMon::
	msgbox HeartSoul_Text_ChoisirPokemonAttaque, MSGBOX_DEFAULT
	setmoverelearnerstate MOVE_RELEARNER_LEVEL_UP_MOVES
	chooseboxmon SELECT_PC_MON_MOVE_RELEARNER
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartSoul_EventScript_CinnabarAttack
	special HasMovesToRelearn
	goto_if_eq VAR_RESULT, FALSE, HeartSoul_ChooseMoves_Feu_NoMove
	msgbox HeartSoul_Text_QuelleAttaque, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto HeartSoul_ChooseMoves_Feu_ChooseMon
HeartSoul_ChooseMoves_Feu_NoMove:
	msgbox HeartSoul_Text_PlusRienAApprendre, MSGBOX_DEFAULT
	goto HeartSoul_ChooseMoves_Feu_ChooseMon
	end

HeartSoul_EventScript_PickOne_Eau::
	dynmultipush HeartSoul_Text_Mon_Eau_0, 0
	dynmultipush HeartSoul_Text_Mon_Eau_1, 1
	dynmultipush HeartSoul_Text_Mon_Eau_2, 2
	dynmultipush HeartSoul_Text_Mon_Eau_3, 3
	dynmultipush HeartSoul_Text_Mon_Eau_4, 4
	dynmultipush HeartSoul_Text_Mon_Eau_5, 5
	dynmultipush HeartSoul_Text_Mon_Eau_6, 6
	dynmultipush HeartSoul_Text_Mon_Eau_7, 7
	dynmultipush HeartSoul_Text_Mon_Eau_8, 8
	dynmultipush HeartSoul_Text_Mon_Eau_9, 9
	dynmultipush HeartSoul_Text_Mon_Eau_10, 10
	dynmultipush HeartSoul_Text_Mon_Eau_11, 11
	dynmultipush HeartSoul_Text_Mon_Eau_12, 12
	dynmultipush HeartSoul_Text_Mon_Eau_13, 13
	dynmultipush HeartSoul_Text_Mon_Eau_14, 14
	dynmultipush HeartSoul_Text_Mon_Eau_15, 15
	dynmultipush HeartSoul_Text_Mon_Eau_16, 16
	dynmultipush HeartSoul_Text_Mon_Eau_17, 17
	dynmultipush HeartSoul_Text_Mon_Eau_18, 18
	dynmultipush HeartSoul_Text_Mon_Eau_19, 19
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_PickOne_Eau
	switch VAR_RESULT
	case 0, HeartSoul_PickOne_Eau_Case0
	case 1, HeartSoul_PickOne_Eau_Case1
	case 2, HeartSoul_PickOne_Eau_Case2
	case 3, HeartSoul_PickOne_Eau_Case3
	case 4, HeartSoul_PickOne_Eau_Case4
	case 5, HeartSoul_PickOne_Eau_Case5
	case 6, HeartSoul_PickOne_Eau_Case6
	case 7, HeartSoul_PickOne_Eau_Case7
	case 8, HeartSoul_PickOne_Eau_Case8
	case 9, HeartSoul_PickOne_Eau_Case9
	case 10, HeartSoul_PickOne_Eau_Case10
	case 11, HeartSoul_PickOne_Eau_Case11
	case 12, HeartSoul_PickOne_Eau_Case12
	case 13, HeartSoul_PickOne_Eau_Case13
	case 14, HeartSoul_PickOne_Eau_Case14
	case 15, HeartSoul_PickOne_Eau_Case15
	case 16, HeartSoul_PickOne_Eau_Case16
	case 17, HeartSoul_PickOne_Eau_Case17
	case 18, HeartSoul_PickOne_Eau_Case18
	case 19, HeartSoul_PickOne_Eau_Case19
	goto HeartSoul_EventScript_PickOne_Eau
	end

HeartSoul_PickOne_Eau_Case0:
	setvar VAR_TEMP_SPECIES, SPECIES_BLASTOISE
	return
HeartSoul_PickOne_Eau_Case1:
	setvar VAR_TEMP_SPECIES, SPECIES_GYARADOS
	return
HeartSoul_PickOne_Eau_Case2:
	setvar VAR_TEMP_SPECIES, SPECIES_VAPOREON
	return
HeartSoul_PickOne_Eau_Case3:
	setvar VAR_TEMP_SPECIES, SPECIES_POLIWRATH
	return
HeartSoul_PickOne_Eau_Case4:
	setvar VAR_TEMP_SPECIES, SPECIES_GOLDUCK
	return
HeartSoul_PickOne_Eau_Case5:
	setvar VAR_TEMP_SPECIES, SPECIES_STARMIE
	return
HeartSoul_PickOne_Eau_Case6:
	setvar VAR_TEMP_SPECIES, SPECIES_LAPRAS
	return
HeartSoul_PickOne_Eau_Case7:
	setvar VAR_TEMP_SPECIES, SPECIES_KINGDRA
	return
HeartSoul_PickOne_Eau_Case8:
	setvar VAR_TEMP_SPECIES, SPECIES_MILOTIC
	return
HeartSoul_PickOne_Eau_Case9:
	setvar VAR_TEMP_SPECIES, SPECIES_FERALIGATR
	return
HeartSoul_PickOne_Eau_Case10:
	setvar VAR_TEMP_SPECIES, SPECIES_SWAMPERT
	return
HeartSoul_PickOne_Eau_Case11:
	setvar VAR_TEMP_SPECIES, SPECIES_EMPOLEON
	return
HeartSoul_PickOne_Eau_Case12:
	setvar VAR_TEMP_SPECIES, SPECIES_GRENINJA
	return
HeartSoul_PickOne_Eau_Case13:
	setvar VAR_TEMP_SPECIES, SPECIES_AZUMARILL
	return
HeartSoul_PickOne_Eau_Case14:
	setvar VAR_TEMP_SPECIES, SPECIES_SLOWBRO
	return
HeartSoul_PickOne_Eau_Case15:
	setvar VAR_TEMP_SPECIES, SPECIES_SEAKING
	return
HeartSoul_PickOne_Eau_Case16:
	setvar VAR_TEMP_SPECIES, SPECIES_KABUTOPS
	return
HeartSoul_PickOne_Eau_Case17:
	setvar VAR_TEMP_SPECIES, SPECIES_OMASTAR
	return
HeartSoul_PickOne_Eau_Case18:
	setvar VAR_TEMP_SPECIES, SPECIES_WHISCASH
	return
HeartSoul_PickOne_Eau_Case19:
	setvar VAR_TEMP_SPECIES, SPECIES_LUDICOLO
	return

HeartSoul_Text_Mon_Eau_0:
	.string "TORTANK$"
HeartSoul_Text_Mon_Eau_1:
	.string "LÉVIATOR$"
HeartSoul_Text_Mon_Eau_2:
	.string "AQUALI$"
HeartSoul_Text_Mon_Eau_3:
	.string "TARTARD$"
HeartSoul_Text_Mon_Eau_4:
	.string "AKWAKWAK$"
HeartSoul_Text_Mon_Eau_5:
	.string "STAROSS$"
HeartSoul_Text_Mon_Eau_6:
	.string "LOKHLASS$"
HeartSoul_Text_Mon_Eau_7:
	.string "HYPOROI$"
HeartSoul_Text_Mon_Eau_8:
	.string "MILOBELLUS$"
HeartSoul_Text_Mon_Eau_9:
	.string "ALIGATUEUR$"
HeartSoul_Text_Mon_Eau_10:
	.string "LAGGRON$"
HeartSoul_Text_Mon_Eau_11:
	.string "PINGOLÉON$"
HeartSoul_Text_Mon_Eau_12:
	.string "AMPHINOBI$"
HeartSoul_Text_Mon_Eau_13:
	.string "AZUMARILL$"
HeartSoul_Text_Mon_Eau_14:
	.string "FLAGADOSS$"
HeartSoul_Text_Mon_Eau_15:
	.string "POISSOROY$"
HeartSoul_Text_Mon_Eau_16:
	.string "KABUTOPS$"
HeartSoul_Text_Mon_Eau_17:
	.string "AMONISTAR$"
HeartSoul_Text_Mon_Eau_18:
	.string "BARBICHA$"
HeartSoul_Text_Mon_Eau_19:
	.string "LUDICOLO$"

HeartSoul_EventScript_ChooseTeam_Eau::
	call HeartSoul_EventScript_PickOne_Eau
	copyvar VAR_0x8004, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Eau_Slot2::
	call HeartSoul_EventScript_PickOne_Eau
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Eau_Slot2
	copyvar VAR_0x8005, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Eau_Slot3::
	call HeartSoul_EventScript_PickOne_Eau
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Eau_Slot3
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Eau_Slot3
	copyvar VAR_0x8006, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Eau_Slot4::
	call HeartSoul_EventScript_PickOne_Eau
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Eau_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Eau_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8006
	goto_if_eq HeartSoul_ChooseTeam_Eau_Slot4
	copyvar VAR_0x8007, VAR_TEMP_SPECIES
	givemon VAR_0x8004, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8005, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8006, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8007, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	setflag FLAG_SYS_POKEMON_GET
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox HeartSoul_Text_ChoisisAttaquesIntro, MSGBOX_DEFAULT
HeartSoul_ChooseMoves_Eau_ChooseMon::
	msgbox HeartSoul_Text_ChoisirPokemonAttaque, MSGBOX_DEFAULT
	setmoverelearnerstate MOVE_RELEARNER_LEVEL_UP_MOVES
	chooseboxmon SELECT_PC_MON_MOVE_RELEARNER
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartSoul_EventScript_CinnabarAttack
	special HasMovesToRelearn
	goto_if_eq VAR_RESULT, FALSE, HeartSoul_ChooseMoves_Eau_NoMove
	msgbox HeartSoul_Text_QuelleAttaque, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto HeartSoul_ChooseMoves_Eau_ChooseMon
HeartSoul_ChooseMoves_Eau_NoMove:
	msgbox HeartSoul_Text_PlusRienAApprendre, MSGBOX_DEFAULT
	goto HeartSoul_ChooseMoves_Eau_ChooseMon
	end

HeartSoul_EventScript_PickOne_Plante::
	dynmultipush HeartSoul_Text_Mon_Plante_0, 0
	dynmultipush HeartSoul_Text_Mon_Plante_1, 1
	dynmultipush HeartSoul_Text_Mon_Plante_2, 2
	dynmultipush HeartSoul_Text_Mon_Plante_3, 3
	dynmultipush HeartSoul_Text_Mon_Plante_4, 4
	dynmultipush HeartSoul_Text_Mon_Plante_5, 5
	dynmultipush HeartSoul_Text_Mon_Plante_6, 6
	dynmultipush HeartSoul_Text_Mon_Plante_7, 7
	dynmultipush HeartSoul_Text_Mon_Plante_8, 8
	dynmultipush HeartSoul_Text_Mon_Plante_9, 9
	dynmultipush HeartSoul_Text_Mon_Plante_10, 10
	dynmultipush HeartSoul_Text_Mon_Plante_11, 11
	dynmultipush HeartSoul_Text_Mon_Plante_12, 12
	dynmultipush HeartSoul_Text_Mon_Plante_13, 13
	dynmultipush HeartSoul_Text_Mon_Plante_14, 14
	dynmultipush HeartSoul_Text_Mon_Plante_15, 15
	dynmultipush HeartSoul_Text_Mon_Plante_16, 16
	dynmultipush HeartSoul_Text_Mon_Plante_17, 17
	dynmultipush HeartSoul_Text_Mon_Plante_18, 18
	dynmultipush HeartSoul_Text_Mon_Plante_19, 19
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_PickOne_Plante
	switch VAR_RESULT
	case 0, HeartSoul_PickOne_Plante_Case0
	case 1, HeartSoul_PickOne_Plante_Case1
	case 2, HeartSoul_PickOne_Plante_Case2
	case 3, HeartSoul_PickOne_Plante_Case3
	case 4, HeartSoul_PickOne_Plante_Case4
	case 5, HeartSoul_PickOne_Plante_Case5
	case 6, HeartSoul_PickOne_Plante_Case6
	case 7, HeartSoul_PickOne_Plante_Case7
	case 8, HeartSoul_PickOne_Plante_Case8
	case 9, HeartSoul_PickOne_Plante_Case9
	case 10, HeartSoul_PickOne_Plante_Case10
	case 11, HeartSoul_PickOne_Plante_Case11
	case 12, HeartSoul_PickOne_Plante_Case12
	case 13, HeartSoul_PickOne_Plante_Case13
	case 14, HeartSoul_PickOne_Plante_Case14
	case 15, HeartSoul_PickOne_Plante_Case15
	case 16, HeartSoul_PickOne_Plante_Case16
	case 17, HeartSoul_PickOne_Plante_Case17
	case 18, HeartSoul_PickOne_Plante_Case18
	case 19, HeartSoul_PickOne_Plante_Case19
	goto HeartSoul_EventScript_PickOne_Plante
	end

HeartSoul_PickOne_Plante_Case0:
	setvar VAR_TEMP_SPECIES, SPECIES_VENUSAUR
	return
HeartSoul_PickOne_Plante_Case1:
	setvar VAR_TEMP_SPECIES, SPECIES_VILEPLUME
	return
HeartSoul_PickOne_Plante_Case2:
	setvar VAR_TEMP_SPECIES, SPECIES_VICTREEBEL
	return
HeartSoul_PickOne_Plante_Case3:
	setvar VAR_TEMP_SPECIES, SPECIES_TANGROWTH
	return
HeartSoul_PickOne_Plante_Case4:
	setvar VAR_TEMP_SPECIES, SPECIES_EXEGGUTOR
	return
HeartSoul_PickOne_Plante_Case5:
	setvar VAR_TEMP_SPECIES, SPECIES_LEAFEON
	return
HeartSoul_PickOne_Plante_Case6:
	setvar VAR_TEMP_SPECIES, SPECIES_SCEPTILE
	return
HeartSoul_PickOne_Plante_Case7:
	setvar VAR_TEMP_SPECIES, SPECIES_TORTERRA
	return
HeartSoul_PickOne_Plante_Case8:
	setvar VAR_TEMP_SPECIES, SPECIES_SERPERIOR
	return
HeartSoul_PickOne_Plante_Case9:
	setvar VAR_TEMP_SPECIES, SPECIES_ROSERADE
	return
HeartSoul_PickOne_Plante_Case10:
	setvar VAR_TEMP_SPECIES, SPECIES_BRELOOM
	return
HeartSoul_PickOne_Plante_Case11:
	setvar VAR_TEMP_SPECIES, SPECIES_CRADILY
	return
HeartSoul_PickOne_Plante_Case12:
	setvar VAR_TEMP_SPECIES, SPECIES_SUNFLORA
	return
HeartSoul_PickOne_Plante_Case13:
	setvar VAR_TEMP_SPECIES, SPECIES_MEGANIUM
	return
HeartSoul_PickOne_Plante_Case14:
	setvar VAR_TEMP_SPECIES, SPECIES_FERROTHORN
	return
HeartSoul_PickOne_Plante_Case15:
	setvar VAR_TEMP_SPECIES, SPECIES_LURANTIS
	return
HeartSoul_PickOne_Plante_Case16:
	setvar VAR_TEMP_SPECIES, SPECIES_TREVENANT
	return
HeartSoul_PickOne_Plante_Case17:
	setvar VAR_TEMP_SPECIES, SPECIES_WHIMSICOTT
	return
HeartSoul_PickOne_Plante_Case18:
	setvar VAR_TEMP_SPECIES, SPECIES_GOGOAT
	return
HeartSoul_PickOne_Plante_Case19:
	setvar VAR_TEMP_SPECIES, SPECIES_ABOMASNOW
	return

HeartSoul_Text_Mon_Plante_0:
	.string "FLORIZARRE$"
HeartSoul_Text_Mon_Plante_1:
	.string "RAFFLESIA$"
HeartSoul_Text_Mon_Plante_2:
	.string "EMPIFLOR$"
HeartSoul_Text_Mon_Plante_3:
	.string "BOULDENEU$"
HeartSoul_Text_Mon_Plante_4:
	.string "NOADKOKO$"
HeartSoul_Text_Mon_Plante_5:
	.string "PHYLLALI$"
HeartSoul_Text_Mon_Plante_6:
	.string "JUNGKO$"
HeartSoul_Text_Mon_Plante_7:
	.string "TORTERRA$"
HeartSoul_Text_Mon_Plante_8:
	.string "MAJASPIC$"
HeartSoul_Text_Mon_Plante_9:
	.string "ROSERADE$"
HeartSoul_Text_Mon_Plante_10:
	.string "CHAPIGNON$"
HeartSoul_Text_Mon_Plante_11:
	.string "VACILYS$"
HeartSoul_Text_Mon_Plante_12:
	.string "HÉLIATRONC$"
HeartSoul_Text_Mon_Plante_13:
	.string "MEGANIUM$"
HeartSoul_Text_Mon_Plante_14:
	.string "NOACIER$"
HeartSoul_Text_Mon_Plante_15:
	.string "FLORAMANTIS$"
HeartSoul_Text_Mon_Plante_16:
	.string "DESSÉLIANDE$"
HeartSoul_Text_Mon_Plante_17:
	.string "FARFADUVET$"
HeartSoul_Text_Mon_Plante_18:
	.string "CHEVROUM$"
HeartSoul_Text_Mon_Plante_19:
	.string "BLIZZAROI$"

HeartSoul_EventScript_ChooseTeam_Plante::
	call HeartSoul_EventScript_PickOne_Plante
	copyvar VAR_0x8004, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Plante_Slot2::
	call HeartSoul_EventScript_PickOne_Plante
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Plante_Slot2
	copyvar VAR_0x8005, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Plante_Slot3::
	call HeartSoul_EventScript_PickOne_Plante
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Plante_Slot3
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Plante_Slot3
	copyvar VAR_0x8006, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Plante_Slot4::
	call HeartSoul_EventScript_PickOne_Plante
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Plante_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Plante_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8006
	goto_if_eq HeartSoul_ChooseTeam_Plante_Slot4
	copyvar VAR_0x8007, VAR_TEMP_SPECIES
	givemon VAR_0x8004, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8005, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8006, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8007, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	setflag FLAG_SYS_POKEMON_GET
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox HeartSoul_Text_ChoisisAttaquesIntro, MSGBOX_DEFAULT
HeartSoul_ChooseMoves_Plante_ChooseMon::
	msgbox HeartSoul_Text_ChoisirPokemonAttaque, MSGBOX_DEFAULT
	setmoverelearnerstate MOVE_RELEARNER_LEVEL_UP_MOVES
	chooseboxmon SELECT_PC_MON_MOVE_RELEARNER
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartSoul_EventScript_CinnabarAttack
	special HasMovesToRelearn
	goto_if_eq VAR_RESULT, FALSE, HeartSoul_ChooseMoves_Plante_NoMove
	msgbox HeartSoul_Text_QuelleAttaque, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto HeartSoul_ChooseMoves_Plante_ChooseMon
HeartSoul_ChooseMoves_Plante_NoMove:
	msgbox HeartSoul_Text_PlusRienAApprendre, MSGBOX_DEFAULT
	goto HeartSoul_ChooseMoves_Plante_ChooseMon
	end

HeartSoul_EventScript_PickOne_Electrik::
	dynmultipush HeartSoul_Text_Mon_Electrik_0, 0
	dynmultipush HeartSoul_Text_Mon_Electrik_1, 1
	dynmultipush HeartSoul_Text_Mon_Electrik_2, 2
	dynmultipush HeartSoul_Text_Mon_Electrik_3, 3
	dynmultipush HeartSoul_Text_Mon_Electrik_4, 4
	dynmultipush HeartSoul_Text_Mon_Electrik_5, 5
	dynmultipush HeartSoul_Text_Mon_Electrik_6, 6
	dynmultipush HeartSoul_Text_Mon_Electrik_7, 7
	dynmultipush HeartSoul_Text_Mon_Electrik_8, 8
	dynmultipush HeartSoul_Text_Mon_Electrik_9, 9
	dynmultipush HeartSoul_Text_Mon_Electrik_10, 10
	dynmultipush HeartSoul_Text_Mon_Electrik_11, 11
	dynmultipush HeartSoul_Text_Mon_Electrik_12, 12
	dynmultipush HeartSoul_Text_Mon_Electrik_13, 13
	dynmultipush HeartSoul_Text_Mon_Electrik_14, 14
	dynmultipush HeartSoul_Text_Mon_Electrik_15, 15
	dynmultipush HeartSoul_Text_Mon_Electrik_16, 16
	dynmultipush HeartSoul_Text_Mon_Electrik_17, 17
	dynmultipush HeartSoul_Text_Mon_Electrik_18, 18
	dynmultipush HeartSoul_Text_Mon_Electrik_19, 19
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_PickOne_Electrik
	switch VAR_RESULT
	case 0, HeartSoul_PickOne_Electrik_Case0
	case 1, HeartSoul_PickOne_Electrik_Case1
	case 2, HeartSoul_PickOne_Electrik_Case2
	case 3, HeartSoul_PickOne_Electrik_Case3
	case 4, HeartSoul_PickOne_Electrik_Case4
	case 5, HeartSoul_PickOne_Electrik_Case5
	case 6, HeartSoul_PickOne_Electrik_Case6
	case 7, HeartSoul_PickOne_Electrik_Case7
	case 8, HeartSoul_PickOne_Electrik_Case8
	case 9, HeartSoul_PickOne_Electrik_Case9
	case 10, HeartSoul_PickOne_Electrik_Case10
	case 11, HeartSoul_PickOne_Electrik_Case11
	case 12, HeartSoul_PickOne_Electrik_Case12
	case 13, HeartSoul_PickOne_Electrik_Case13
	case 14, HeartSoul_PickOne_Electrik_Case14
	case 15, HeartSoul_PickOne_Electrik_Case15
	case 16, HeartSoul_PickOne_Electrik_Case16
	case 17, HeartSoul_PickOne_Electrik_Case17
	case 18, HeartSoul_PickOne_Electrik_Case18
	case 19, HeartSoul_PickOne_Electrik_Case19
	goto HeartSoul_EventScript_PickOne_Electrik
	end

HeartSoul_PickOne_Electrik_Case0:
	setvar VAR_TEMP_SPECIES, SPECIES_RAICHU
	return
HeartSoul_PickOne_Electrik_Case1:
	setvar VAR_TEMP_SPECIES, SPECIES_ELECTRODE
	return
HeartSoul_PickOne_Electrik_Case2:
	setvar VAR_TEMP_SPECIES, SPECIES_ELECTIVIRE
	return
HeartSoul_PickOne_Electrik_Case3:
	setvar VAR_TEMP_SPECIES, SPECIES_JOLTEON
	return
HeartSoul_PickOne_Electrik_Case4:
	setvar VAR_TEMP_SPECIES, SPECIES_AMPHAROS
	return
HeartSoul_PickOne_Electrik_Case5:
	setvar VAR_TEMP_SPECIES, SPECIES_LUXRAY
	return
HeartSoul_PickOne_Electrik_Case6:
	setvar VAR_TEMP_SPECIES, SPECIES_MANECTRIC
	return
HeartSoul_PickOne_Electrik_Case7:
	setvar VAR_TEMP_SPECIES, SPECIES_MAGNEZONE
	return
HeartSoul_PickOne_Electrik_Case8:
	setvar VAR_TEMP_SPECIES, SPECIES_ZEBSTRIKA
	return
HeartSoul_PickOne_Electrik_Case9:
	setvar VAR_TEMP_SPECIES, SPECIES_EELEKTROSS
	return
HeartSoul_PickOne_Electrik_Case10:
	setvar VAR_TEMP_SPECIES, SPECIES_ROTOM
	return
HeartSoul_PickOne_Electrik_Case11:
	setvar VAR_TEMP_SPECIES, SPECIES_GALVANTULA
	return
HeartSoul_PickOne_Electrik_Case12:
	setvar VAR_TEMP_SPECIES, SPECIES_HELIOLISK
	return
HeartSoul_PickOne_Electrik_Case13:
	setvar VAR_TEMP_SPECIES, SPECIES_LANTURN
	return
HeartSoul_PickOne_Electrik_Case14:
	setvar VAR_TEMP_SPECIES, SPECIES_EMOLGA
	return
HeartSoul_PickOne_Electrik_Case15:
	setvar VAR_TEMP_SPECIES, SPECIES_VIKAVOLT
	return
HeartSoul_PickOne_Electrik_Case16:
	setvar VAR_TEMP_SPECIES, SPECIES_TOXTRICITY
	return
HeartSoul_PickOne_Electrik_Case17:
	setvar VAR_TEMP_SPECIES, SPECIES_DEDENNE
	return
HeartSoul_PickOne_Electrik_Case18:
	setvar VAR_TEMP_SPECIES, SPECIES_PIKACHU
	return
HeartSoul_PickOne_Electrik_Case19:
	setvar VAR_TEMP_SPECIES, SPECIES_STUNFISK
	return

HeartSoul_Text_Mon_Electrik_0:
	.string "RAICHU$"
HeartSoul_Text_Mon_Electrik_1:
	.string "ELECTRODE$"
HeartSoul_Text_Mon_Electrik_2:
	.string "ÉLÉKABLE$"
HeartSoul_Text_Mon_Electrik_3:
	.string "VOLTALI$"
HeartSoul_Text_Mon_Electrik_4:
	.string "PHARAMP$"
HeartSoul_Text_Mon_Electrik_5:
	.string "LUXRAY$"
HeartSoul_Text_Mon_Electrik_6:
	.string "ÉLECSPRINT$"
HeartSoul_Text_Mon_Electrik_7:
	.string "MAGNÉZONE$"
HeartSoul_Text_Mon_Electrik_8:
	.string "ZÉBLITZ$"
HeartSoul_Text_Mon_Electrik_9:
	.string "OHMASSACRE$"
HeartSoul_Text_Mon_Electrik_10:
	.string "MOTISMA$"
HeartSoul_Text_Mon_Electrik_11:
	.string "MYGAVOLT$"
HeartSoul_Text_Mon_Electrik_12:
	.string "IGUOLTA$"
HeartSoul_Text_Mon_Electrik_13:
	.string "LANTURN$"
HeartSoul_Text_Mon_Electrik_14:
	.string "EMOLGA$"
HeartSoul_Text_Mon_Electrik_15:
	.string "LUCANON$"
HeartSoul_Text_Mon_Electrik_16:
	.string "SALARSEN$"
HeartSoul_Text_Mon_Electrik_17:
	.string "DEDENNE$"
HeartSoul_Text_Mon_Electrik_18:
	.string "PIKACHU$"
HeartSoul_Text_Mon_Electrik_19:
	.string "LIMONDE$"

HeartSoul_EventScript_ChooseTeam_Electrik::
	call HeartSoul_EventScript_PickOne_Electrik
	copyvar VAR_0x8004, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Electrik_Slot2::
	call HeartSoul_EventScript_PickOne_Electrik
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Electrik_Slot2
	copyvar VAR_0x8005, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Electrik_Slot3::
	call HeartSoul_EventScript_PickOne_Electrik
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Electrik_Slot3
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Electrik_Slot3
	copyvar VAR_0x8006, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Electrik_Slot4::
	call HeartSoul_EventScript_PickOne_Electrik
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Electrik_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Electrik_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8006
	goto_if_eq HeartSoul_ChooseTeam_Electrik_Slot4
	copyvar VAR_0x8007, VAR_TEMP_SPECIES
	givemon VAR_0x8004, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8005, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8006, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8007, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	setflag FLAG_SYS_POKEMON_GET
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox HeartSoul_Text_ChoisisAttaquesIntro, MSGBOX_DEFAULT
HeartSoul_ChooseMoves_Electrik_ChooseMon::
	msgbox HeartSoul_Text_ChoisirPokemonAttaque, MSGBOX_DEFAULT
	setmoverelearnerstate MOVE_RELEARNER_LEVEL_UP_MOVES
	chooseboxmon SELECT_PC_MON_MOVE_RELEARNER
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartSoul_EventScript_CinnabarAttack
	special HasMovesToRelearn
	goto_if_eq VAR_RESULT, FALSE, HeartSoul_ChooseMoves_Electrik_NoMove
	msgbox HeartSoul_Text_QuelleAttaque, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto HeartSoul_ChooseMoves_Electrik_ChooseMon
HeartSoul_ChooseMoves_Electrik_NoMove:
	msgbox HeartSoul_Text_PlusRienAApprendre, MSGBOX_DEFAULT
	goto HeartSoul_ChooseMoves_Electrik_ChooseMon
	end

HeartSoul_EventScript_PickOne_Glace::
	dynmultipush HeartSoul_Text_Mon_Glace_0, 0
	dynmultipush HeartSoul_Text_Mon_Glace_1, 1
	dynmultipush HeartSoul_Text_Mon_Glace_2, 2
	dynmultipush HeartSoul_Text_Mon_Glace_3, 3
	dynmultipush HeartSoul_Text_Mon_Glace_4, 4
	dynmultipush HeartSoul_Text_Mon_Glace_5, 5
	dynmultipush HeartSoul_Text_Mon_Glace_6, 6
	dynmultipush HeartSoul_Text_Mon_Glace_7, 7
	dynmultipush HeartSoul_Text_Mon_Glace_8, 8
	dynmultipush HeartSoul_Text_Mon_Glace_9, 9
	dynmultipush HeartSoul_Text_Mon_Glace_10, 10
	dynmultipush HeartSoul_Text_Mon_Glace_11, 11
	dynmultipush HeartSoul_Text_Mon_Glace_12, 12
	dynmultipush HeartSoul_Text_Mon_Glace_13, 13
	dynmultipush HeartSoul_Text_Mon_Glace_14, 14
	dynmultipush HeartSoul_Text_Mon_Glace_15, 15
	dynmultipush HeartSoul_Text_Mon_Glace_16, 16
	dynmultipush HeartSoul_Text_Mon_Glace_17, 17
	dynmultipush HeartSoul_Text_Mon_Glace_18, 18
	dynmultipush HeartSoul_Text_Mon_Glace_19, 19
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_PickOne_Glace
	switch VAR_RESULT
	case 0, HeartSoul_PickOne_Glace_Case0
	case 1, HeartSoul_PickOne_Glace_Case1
	case 2, HeartSoul_PickOne_Glace_Case2
	case 3, HeartSoul_PickOne_Glace_Case3
	case 4, HeartSoul_PickOne_Glace_Case4
	case 5, HeartSoul_PickOne_Glace_Case5
	case 6, HeartSoul_PickOne_Glace_Case6
	case 7, HeartSoul_PickOne_Glace_Case7
	case 8, HeartSoul_PickOne_Glace_Case8
	case 9, HeartSoul_PickOne_Glace_Case9
	case 10, HeartSoul_PickOne_Glace_Case10
	case 11, HeartSoul_PickOne_Glace_Case11
	case 12, HeartSoul_PickOne_Glace_Case12
	case 13, HeartSoul_PickOne_Glace_Case13
	case 14, HeartSoul_PickOne_Glace_Case14
	case 15, HeartSoul_PickOne_Glace_Case15
	case 16, HeartSoul_PickOne_Glace_Case16
	case 17, HeartSoul_PickOne_Glace_Case17
	case 18, HeartSoul_PickOne_Glace_Case18
	case 19, HeartSoul_PickOne_Glace_Case19
	goto HeartSoul_EventScript_PickOne_Glace
	end

HeartSoul_PickOne_Glace_Case0:
	setvar VAR_TEMP_SPECIES, SPECIES_GLACEON
	return
HeartSoul_PickOne_Glace_Case1:
	setvar VAR_TEMP_SPECIES, SPECIES_WEAVILE
	return
HeartSoul_PickOne_Glace_Case2:
	setvar VAR_TEMP_SPECIES, SPECIES_FROSLASS
	return
HeartSoul_PickOne_Glace_Case3:
	setvar VAR_TEMP_SPECIES, SPECIES_MAMOSWINE
	return
HeartSoul_PickOne_Glace_Case4:
	setvar VAR_TEMP_SPECIES, SPECIES_WALREIN
	return
HeartSoul_PickOne_Glace_Case5:
	setvar VAR_TEMP_SPECIES, SPECIES_CLOYSTER
	return
HeartSoul_PickOne_Glace_Case6:
	setvar VAR_TEMP_SPECIES, SPECIES_ABOMASNOW
	return
HeartSoul_PickOne_Glace_Case7:
	setvar VAR_TEMP_SPECIES, SPECIES_VANILLUXE
	return
HeartSoul_PickOne_Glace_Case8:
	setvar VAR_TEMP_SPECIES, SPECIES_BEARTIC
	return
HeartSoul_PickOne_Glace_Case9:
	setvar VAR_TEMP_SPECIES, SPECIES_AVALUGG
	return
HeartSoul_PickOne_Glace_Case10:
	setvar VAR_TEMP_SPECIES, SPECIES_CRYOGONAL
	return
HeartSoul_PickOne_Glace_Case11:
	setvar VAR_TEMP_SPECIES, SPECIES_NINETALES_ALOLA
	return
HeartSoul_PickOne_Glace_Case12:
	setvar VAR_TEMP_SPECIES, SPECIES_SANDSLASH_ALOLA
	return
HeartSoul_PickOne_Glace_Case13:
	setvar VAR_TEMP_SPECIES, SPECIES_JYNX
	return
HeartSoul_PickOne_Glace_Case14:
	setvar VAR_TEMP_SPECIES, SPECIES_AURORUS
	return
HeartSoul_PickOne_Glace_Case15:
	setvar VAR_TEMP_SPECIES, SPECIES_PILOSWINE
	return
HeartSoul_PickOne_Glace_Case16:
	setvar VAR_TEMP_SPECIES, SPECIES_DELIBIRD
	return
HeartSoul_PickOne_Glace_Case17:
	setvar VAR_TEMP_SPECIES, SPECIES_FROSMOTH
	return
HeartSoul_PickOne_Glace_Case18:
	setvar VAR_TEMP_SPECIES, SPECIES_LAPRAS
	return
HeartSoul_PickOne_Glace_Case19:
	setvar VAR_TEMP_SPECIES, SPECIES_SNEASEL
	return

HeartSoul_Text_Mon_Glace_0:
	.string "GIVRALI$"
HeartSoul_Text_Mon_Glace_1:
	.string "DIMORET$"
HeartSoul_Text_Mon_Glace_2:
	.string "MOMARTIK$"
HeartSoul_Text_Mon_Glace_3:
	.string "MAMMOCHON$"
HeartSoul_Text_Mon_Glace_4:
	.string "KAIMORSE$"
HeartSoul_Text_Mon_Glace_5:
	.string "CRUSTABRI$"
HeartSoul_Text_Mon_Glace_6:
	.string "BLIZZAROI$"
HeartSoul_Text_Mon_Glace_7:
	.string "SORBOUBOUL$"
HeartSoul_Text_Mon_Glace_8:
	.string "POLAGRIFFE$"
HeartSoul_Text_Mon_Glace_9:
	.string "SÉRACRAWL$"
HeartSoul_Text_Mon_Glace_10:
	.string "HEXAGEL$"
HeartSoul_Text_Mon_Glace_11:
	.string "FEUNARD D'ALOLA$"
HeartSoul_Text_Mon_Glace_12:
	.string "SABLAIREAU D'ALOLA$"
HeartSoul_Text_Mon_Glace_13:
	.string "LIPPOUTOU$"
HeartSoul_Text_Mon_Glace_14:
	.string "DRAGMARA$"
HeartSoul_Text_Mon_Glace_15:
	.string "COCHIGNON$"
HeartSoul_Text_Mon_Glace_16:
	.string "CADOIZO$"
HeartSoul_Text_Mon_Glace_17:
	.string "BELDENEIGE$"
HeartSoul_Text_Mon_Glace_18:
	.string "LOKHLASS$"
HeartSoul_Text_Mon_Glace_19:
	.string "FARFURET$"

HeartSoul_EventScript_ChooseTeam_Glace::
	call HeartSoul_EventScript_PickOne_Glace
	copyvar VAR_0x8004, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Glace_Slot2::
	call HeartSoul_EventScript_PickOne_Glace
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Glace_Slot2
	copyvar VAR_0x8005, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Glace_Slot3::
	call HeartSoul_EventScript_PickOne_Glace
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Glace_Slot3
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Glace_Slot3
	copyvar VAR_0x8006, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Glace_Slot4::
	call HeartSoul_EventScript_PickOne_Glace
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Glace_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Glace_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8006
	goto_if_eq HeartSoul_ChooseTeam_Glace_Slot4
	copyvar VAR_0x8007, VAR_TEMP_SPECIES
	givemon VAR_0x8004, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8005, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8006, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8007, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	setflag FLAG_SYS_POKEMON_GET
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox HeartSoul_Text_ChoisisAttaquesIntro, MSGBOX_DEFAULT
HeartSoul_ChooseMoves_Glace_ChooseMon::
	msgbox HeartSoul_Text_ChoisirPokemonAttaque, MSGBOX_DEFAULT
	setmoverelearnerstate MOVE_RELEARNER_LEVEL_UP_MOVES
	chooseboxmon SELECT_PC_MON_MOVE_RELEARNER
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartSoul_EventScript_CinnabarAttack
	special HasMovesToRelearn
	goto_if_eq VAR_RESULT, FALSE, HeartSoul_ChooseMoves_Glace_NoMove
	msgbox HeartSoul_Text_QuelleAttaque, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto HeartSoul_ChooseMoves_Glace_ChooseMon
HeartSoul_ChooseMoves_Glace_NoMove:
	msgbox HeartSoul_Text_PlusRienAApprendre, MSGBOX_DEFAULT
	goto HeartSoul_ChooseMoves_Glace_ChooseMon
	end

HeartSoul_EventScript_PickOne_Combat::
	dynmultipush HeartSoul_Text_Mon_Combat_0, 0
	dynmultipush HeartSoul_Text_Mon_Combat_1, 1
	dynmultipush HeartSoul_Text_Mon_Combat_2, 2
	dynmultipush HeartSoul_Text_Mon_Combat_3, 3
	dynmultipush HeartSoul_Text_Mon_Combat_4, 4
	dynmultipush HeartSoul_Text_Mon_Combat_5, 5
	dynmultipush HeartSoul_Text_Mon_Combat_6, 6
	dynmultipush HeartSoul_Text_Mon_Combat_7, 7
	dynmultipush HeartSoul_Text_Mon_Combat_8, 8
	dynmultipush HeartSoul_Text_Mon_Combat_9, 9
	dynmultipush HeartSoul_Text_Mon_Combat_10, 10
	dynmultipush HeartSoul_Text_Mon_Combat_11, 11
	dynmultipush HeartSoul_Text_Mon_Combat_12, 12
	dynmultipush HeartSoul_Text_Mon_Combat_13, 13
	dynmultipush HeartSoul_Text_Mon_Combat_14, 14
	dynmultipush HeartSoul_Text_Mon_Combat_15, 15
	dynmultipush HeartSoul_Text_Mon_Combat_16, 16
	dynmultipush HeartSoul_Text_Mon_Combat_17, 17
	dynmultipush HeartSoul_Text_Mon_Combat_18, 18
	dynmultipush HeartSoul_Text_Mon_Combat_19, 19
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_PickOne_Combat
	switch VAR_RESULT
	case 0, HeartSoul_PickOne_Combat_Case0
	case 1, HeartSoul_PickOne_Combat_Case1
	case 2, HeartSoul_PickOne_Combat_Case2
	case 3, HeartSoul_PickOne_Combat_Case3
	case 4, HeartSoul_PickOne_Combat_Case4
	case 5, HeartSoul_PickOne_Combat_Case5
	case 6, HeartSoul_PickOne_Combat_Case6
	case 7, HeartSoul_PickOne_Combat_Case7
	case 8, HeartSoul_PickOne_Combat_Case8
	case 9, HeartSoul_PickOne_Combat_Case9
	case 10, HeartSoul_PickOne_Combat_Case10
	case 11, HeartSoul_PickOne_Combat_Case11
	case 12, HeartSoul_PickOne_Combat_Case12
	case 13, HeartSoul_PickOne_Combat_Case13
	case 14, HeartSoul_PickOne_Combat_Case14
	case 15, HeartSoul_PickOne_Combat_Case15
	case 16, HeartSoul_PickOne_Combat_Case16
	case 17, HeartSoul_PickOne_Combat_Case17
	case 18, HeartSoul_PickOne_Combat_Case18
	case 19, HeartSoul_PickOne_Combat_Case19
	goto HeartSoul_EventScript_PickOne_Combat
	end

HeartSoul_PickOne_Combat_Case0:
	setvar VAR_TEMP_SPECIES, SPECIES_MACHAMP
	return
HeartSoul_PickOne_Combat_Case1:
	setvar VAR_TEMP_SPECIES, SPECIES_HITMONLEE
	return
HeartSoul_PickOne_Combat_Case2:
	setvar VAR_TEMP_SPECIES, SPECIES_HITMONCHAN
	return
HeartSoul_PickOne_Combat_Case3:
	setvar VAR_TEMP_SPECIES, SPECIES_HITMONTOP
	return
HeartSoul_PickOne_Combat_Case4:
	setvar VAR_TEMP_SPECIES, SPECIES_HERACROSS
	return
HeartSoul_PickOne_Combat_Case5:
	setvar VAR_TEMP_SPECIES, SPECIES_LUCARIO
	return
HeartSoul_PickOne_Combat_Case6:
	setvar VAR_TEMP_SPECIES, SPECIES_TOXICROAK
	return
HeartSoul_PickOne_Combat_Case7:
	setvar VAR_TEMP_SPECIES, SPECIES_GALLADE
	return
HeartSoul_PickOne_Combat_Case8:
	setvar VAR_TEMP_SPECIES, SPECIES_CONKELDURR
	return
HeartSoul_PickOne_Combat_Case9:
	setvar VAR_TEMP_SPECIES, SPECIES_THROH
	return
HeartSoul_PickOne_Combat_Case10:
	setvar VAR_TEMP_SPECIES, SPECIES_SAWK
	return
HeartSoul_PickOne_Combat_Case11:
	setvar VAR_TEMP_SPECIES, SPECIES_SCRAFTY
	return
HeartSoul_PickOne_Combat_Case12:
	setvar VAR_TEMP_SPECIES, SPECIES_PANGORO
	return
HeartSoul_PickOne_Combat_Case13:
	setvar VAR_TEMP_SPECIES, SPECIES_PASSIMIAN
	return
HeartSoul_PickOne_Combat_Case14:
	setvar VAR_TEMP_SPECIES, SPECIES_SIRFETCHD
	return
HeartSoul_PickOne_Combat_Case15:
	setvar VAR_TEMP_SPECIES, SPECIES_PRIMEAPE
	return
HeartSoul_PickOne_Combat_Case16:
	setvar VAR_TEMP_SPECIES, SPECIES_POLIWRATH
	return
HeartSoul_PickOne_Combat_Case17:
	setvar VAR_TEMP_SPECIES, SPECIES_BLAZIKEN
	return
HeartSoul_PickOne_Combat_Case18:
	setvar VAR_TEMP_SPECIES, SPECIES_INFERNAPE
	return
HeartSoul_PickOne_Combat_Case19:
	setvar VAR_TEMP_SPECIES, SPECIES_EMBOAR
	return

HeartSoul_Text_Mon_Combat_0:
	.string "MACKOGNEUR$"
HeartSoul_Text_Mon_Combat_1:
	.string "KICKLEE$"
HeartSoul_Text_Mon_Combat_2:
	.string "TYGNON$"
HeartSoul_Text_Mon_Combat_3:
	.string "KAPOERA$"
HeartSoul_Text_Mon_Combat_4:
	.string "SCARHINO$"
HeartSoul_Text_Mon_Combat_5:
	.string "LUCARIO$"
HeartSoul_Text_Mon_Combat_6:
	.string "COATOX$"
HeartSoul_Text_Mon_Combat_7:
	.string "GALLAME$"
HeartSoul_Text_Mon_Combat_8:
	.string "BÉTOCHEF$"
HeartSoul_Text_Mon_Combat_9:
	.string "JUDOKRAK$"
HeartSoul_Text_Mon_Combat_10:
	.string "KARACLÉE$"
HeartSoul_Text_Mon_Combat_11:
	.string "BAGGAÏD$"
HeartSoul_Text_Mon_Combat_12:
	.string "PANDARBARE$"
HeartSoul_Text_Mon_Combat_13:
	.string "QUARTERMAC$"
HeartSoul_Text_Mon_Combat_14:
	.string "PALARTICHO$"
HeartSoul_Text_Mon_Combat_15:
	.string "COLOSSINGE$"
HeartSoul_Text_Mon_Combat_16:
	.string "TARTARD$"
HeartSoul_Text_Mon_Combat_17:
	.string "BRASÉGALI$"
HeartSoul_Text_Mon_Combat_18:
	.string "SIMIABRAZ$"
HeartSoul_Text_Mon_Combat_19:
	.string "ROITIFLAM$"

HeartSoul_EventScript_ChooseTeam_Combat::
	call HeartSoul_EventScript_PickOne_Combat
	copyvar VAR_0x8004, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Combat_Slot2::
	call HeartSoul_EventScript_PickOne_Combat
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Combat_Slot2
	copyvar VAR_0x8005, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Combat_Slot3::
	call HeartSoul_EventScript_PickOne_Combat
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Combat_Slot3
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Combat_Slot3
	copyvar VAR_0x8006, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Combat_Slot4::
	call HeartSoul_EventScript_PickOne_Combat
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Combat_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Combat_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8006
	goto_if_eq HeartSoul_ChooseTeam_Combat_Slot4
	copyvar VAR_0x8007, VAR_TEMP_SPECIES
	givemon VAR_0x8004, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8005, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8006, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8007, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	setflag FLAG_SYS_POKEMON_GET
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox HeartSoul_Text_ChoisisAttaquesIntro, MSGBOX_DEFAULT
HeartSoul_ChooseMoves_Combat_ChooseMon::
	msgbox HeartSoul_Text_ChoisirPokemonAttaque, MSGBOX_DEFAULT
	setmoverelearnerstate MOVE_RELEARNER_LEVEL_UP_MOVES
	chooseboxmon SELECT_PC_MON_MOVE_RELEARNER
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartSoul_EventScript_CinnabarAttack
	special HasMovesToRelearn
	goto_if_eq VAR_RESULT, FALSE, HeartSoul_ChooseMoves_Combat_NoMove
	msgbox HeartSoul_Text_QuelleAttaque, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto HeartSoul_ChooseMoves_Combat_ChooseMon
HeartSoul_ChooseMoves_Combat_NoMove:
	msgbox HeartSoul_Text_PlusRienAApprendre, MSGBOX_DEFAULT
	goto HeartSoul_ChooseMoves_Combat_ChooseMon
	end

HeartSoul_EventScript_PickOne_Poison::
	dynmultipush HeartSoul_Text_Mon_Poison_0, 0
	dynmultipush HeartSoul_Text_Mon_Poison_1, 1
	dynmultipush HeartSoul_Text_Mon_Poison_2, 2
	dynmultipush HeartSoul_Text_Mon_Poison_3, 3
	dynmultipush HeartSoul_Text_Mon_Poison_4, 4
	dynmultipush HeartSoul_Text_Mon_Poison_5, 5
	dynmultipush HeartSoul_Text_Mon_Poison_6, 6
	dynmultipush HeartSoul_Text_Mon_Poison_7, 7
	dynmultipush HeartSoul_Text_Mon_Poison_8, 8
	dynmultipush HeartSoul_Text_Mon_Poison_9, 9
	dynmultipush HeartSoul_Text_Mon_Poison_10, 10
	dynmultipush HeartSoul_Text_Mon_Poison_11, 11
	dynmultipush HeartSoul_Text_Mon_Poison_12, 12
	dynmultipush HeartSoul_Text_Mon_Poison_13, 13
	dynmultipush HeartSoul_Text_Mon_Poison_14, 14
	dynmultipush HeartSoul_Text_Mon_Poison_15, 15
	dynmultipush HeartSoul_Text_Mon_Poison_16, 16
	dynmultipush HeartSoul_Text_Mon_Poison_17, 17
	dynmultipush HeartSoul_Text_Mon_Poison_18, 18
	dynmultipush HeartSoul_Text_Mon_Poison_19, 19
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_PickOne_Poison
	switch VAR_RESULT
	case 0, HeartSoul_PickOne_Poison_Case0
	case 1, HeartSoul_PickOne_Poison_Case1
	case 2, HeartSoul_PickOne_Poison_Case2
	case 3, HeartSoul_PickOne_Poison_Case3
	case 4, HeartSoul_PickOne_Poison_Case4
	case 5, HeartSoul_PickOne_Poison_Case5
	case 6, HeartSoul_PickOne_Poison_Case6
	case 7, HeartSoul_PickOne_Poison_Case7
	case 8, HeartSoul_PickOne_Poison_Case8
	case 9, HeartSoul_PickOne_Poison_Case9
	case 10, HeartSoul_PickOne_Poison_Case10
	case 11, HeartSoul_PickOne_Poison_Case11
	case 12, HeartSoul_PickOne_Poison_Case12
	case 13, HeartSoul_PickOne_Poison_Case13
	case 14, HeartSoul_PickOne_Poison_Case14
	case 15, HeartSoul_PickOne_Poison_Case15
	case 16, HeartSoul_PickOne_Poison_Case16
	case 17, HeartSoul_PickOne_Poison_Case17
	case 18, HeartSoul_PickOne_Poison_Case18
	case 19, HeartSoul_PickOne_Poison_Case19
	goto HeartSoul_EventScript_PickOne_Poison
	end

HeartSoul_PickOne_Poison_Case0:
	setvar VAR_TEMP_SPECIES, SPECIES_MUK
	return
HeartSoul_PickOne_Poison_Case1:
	setvar VAR_TEMP_SPECIES, SPECIES_WEEZING
	return
HeartSoul_PickOne_Poison_Case2:
	setvar VAR_TEMP_SPECIES, SPECIES_CROBAT
	return
HeartSoul_PickOne_Poison_Case3:
	setvar VAR_TEMP_SPECIES, SPECIES_VENOMOTH
	return
HeartSoul_PickOne_Poison_Case4:
	setvar VAR_TEMP_SPECIES, SPECIES_NIDOKING
	return
HeartSoul_PickOne_Poison_Case5:
	setvar VAR_TEMP_SPECIES, SPECIES_NIDOQUEEN
	return
HeartSoul_PickOne_Poison_Case6:
	setvar VAR_TEMP_SPECIES, SPECIES_DRAPION
	return
HeartSoul_PickOne_Poison_Case7:
	setvar VAR_TEMP_SPECIES, SPECIES_SKUNTANK
	return
HeartSoul_PickOne_Poison_Case8:
	setvar VAR_TEMP_SPECIES, SPECIES_TOXAPEX
	return
HeartSoul_PickOne_Poison_Case9:
	setvar VAR_TEMP_SPECIES, SPECIES_NAGANADEL
	return
HeartSoul_PickOne_Poison_Case10:
	setvar VAR_TEMP_SPECIES, SPECIES_SEVIPER
	return
HeartSoul_PickOne_Poison_Case11:
	setvar VAR_TEMP_SPECIES, SPECIES_ARBOK
	return
HeartSoul_PickOne_Poison_Case12:
	setvar VAR_TEMP_SPECIES, SPECIES_BEEDRILL
	return
HeartSoul_PickOne_Poison_Case13:
	setvar VAR_TEMP_SPECIES, SPECIES_TENTACRUEL
	return
HeartSoul_PickOne_Poison_Case14:
	setvar VAR_TEMP_SPECIES, SPECIES_SALAZZLE
	return
HeartSoul_PickOne_Poison_Case15:
	setvar VAR_TEMP_SPECIES, SPECIES_AMOONGUSS
	return
HeartSoul_PickOne_Poison_Case16:
	setvar VAR_TEMP_SPECIES, SPECIES_TOXICROAK
	return
HeartSoul_PickOne_Poison_Case17:
	setvar VAR_TEMP_SPECIES, SPECIES_VILEPLUME
	return
HeartSoul_PickOne_Poison_Case18:
	setvar VAR_TEMP_SPECIES, SPECIES_VICTREEBEL
	return
HeartSoul_PickOne_Poison_Case19:
	setvar VAR_TEMP_SPECIES, SPECIES_GENGAR
	return

HeartSoul_Text_Mon_Poison_0:
	.string "GROTADMORV$"
HeartSoul_Text_Mon_Poison_1:
	.string "SMOGOGO$"
HeartSoul_Text_Mon_Poison_2:
	.string "NOSTENFER$"
HeartSoul_Text_Mon_Poison_3:
	.string "AÉROMITE$"
HeartSoul_Text_Mon_Poison_4:
	.string "NIDOKING$"
HeartSoul_Text_Mon_Poison_5:
	.string "NIDOQUEEN$"
HeartSoul_Text_Mon_Poison_6:
	.string "DRASCORE$"
HeartSoul_Text_Mon_Poison_7:
	.string "MOUFFLAIR$"
HeartSoul_Text_Mon_Poison_8:
	.string "PRÉDASTÉRIE$"
HeartSoul_Text_Mon_Poison_9:
	.string "MANDRILLON$"
HeartSoul_Text_Mon_Poison_10:
	.string "SEVIPER$"
HeartSoul_Text_Mon_Poison_11:
	.string "ARBOK$"
HeartSoul_Text_Mon_Poison_12:
	.string "DARDARGNAN$"
HeartSoul_Text_Mon_Poison_13:
	.string "TENTACRUEL$"
HeartSoul_Text_Mon_Poison_14:
	.string "MALAMANDRE$"
HeartSoul_Text_Mon_Poison_15:
	.string "GAULET$"
HeartSoul_Text_Mon_Poison_16:
	.string "COATOX$"
HeartSoul_Text_Mon_Poison_17:
	.string "RAFFLESIA$"
HeartSoul_Text_Mon_Poison_18:
	.string "EMPIFLOR$"
HeartSoul_Text_Mon_Poison_19:
	.string "ECTOPLASMA$"

HeartSoul_EventScript_ChooseTeam_Poison::
	call HeartSoul_EventScript_PickOne_Poison
	copyvar VAR_0x8004, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Poison_Slot2::
	call HeartSoul_EventScript_PickOne_Poison
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Poison_Slot2
	copyvar VAR_0x8005, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Poison_Slot3::
	call HeartSoul_EventScript_PickOne_Poison
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Poison_Slot3
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Poison_Slot3
	copyvar VAR_0x8006, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Poison_Slot4::
	call HeartSoul_EventScript_PickOne_Poison
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Poison_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Poison_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8006
	goto_if_eq HeartSoul_ChooseTeam_Poison_Slot4
	copyvar VAR_0x8007, VAR_TEMP_SPECIES
	givemon VAR_0x8004, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8005, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8006, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8007, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	setflag FLAG_SYS_POKEMON_GET
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox HeartSoul_Text_ChoisisAttaquesIntro, MSGBOX_DEFAULT
HeartSoul_ChooseMoves_Poison_ChooseMon::
	msgbox HeartSoul_Text_ChoisirPokemonAttaque, MSGBOX_DEFAULT
	setmoverelearnerstate MOVE_RELEARNER_LEVEL_UP_MOVES
	chooseboxmon SELECT_PC_MON_MOVE_RELEARNER
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartSoul_EventScript_CinnabarAttack
	special HasMovesToRelearn
	goto_if_eq VAR_RESULT, FALSE, HeartSoul_ChooseMoves_Poison_NoMove
	msgbox HeartSoul_Text_QuelleAttaque, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto HeartSoul_ChooseMoves_Poison_ChooseMon
HeartSoul_ChooseMoves_Poison_NoMove:
	msgbox HeartSoul_Text_PlusRienAApprendre, MSGBOX_DEFAULT
	goto HeartSoul_ChooseMoves_Poison_ChooseMon
	end

HeartSoul_EventScript_PickOne_Sol::
	dynmultipush HeartSoul_Text_Mon_Sol_0, 0
	dynmultipush HeartSoul_Text_Mon_Sol_1, 1
	dynmultipush HeartSoul_Text_Mon_Sol_2, 2
	dynmultipush HeartSoul_Text_Mon_Sol_3, 3
	dynmultipush HeartSoul_Text_Mon_Sol_4, 4
	dynmultipush HeartSoul_Text_Mon_Sol_5, 5
	dynmultipush HeartSoul_Text_Mon_Sol_6, 6
	dynmultipush HeartSoul_Text_Mon_Sol_7, 7
	dynmultipush HeartSoul_Text_Mon_Sol_8, 8
	dynmultipush HeartSoul_Text_Mon_Sol_9, 9
	dynmultipush HeartSoul_Text_Mon_Sol_10, 10
	dynmultipush HeartSoul_Text_Mon_Sol_11, 11
	dynmultipush HeartSoul_Text_Mon_Sol_12, 12
	dynmultipush HeartSoul_Text_Mon_Sol_13, 13
	dynmultipush HeartSoul_Text_Mon_Sol_14, 14
	dynmultipush HeartSoul_Text_Mon_Sol_15, 15
	dynmultipush HeartSoul_Text_Mon_Sol_16, 16
	dynmultipush HeartSoul_Text_Mon_Sol_17, 17
	dynmultipush HeartSoul_Text_Mon_Sol_18, 18
	dynmultipush HeartSoul_Text_Mon_Sol_19, 19
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_PickOne_Sol
	switch VAR_RESULT
	case 0, HeartSoul_PickOne_Sol_Case0
	case 1, HeartSoul_PickOne_Sol_Case1
	case 2, HeartSoul_PickOne_Sol_Case2
	case 3, HeartSoul_PickOne_Sol_Case3
	case 4, HeartSoul_PickOne_Sol_Case4
	case 5, HeartSoul_PickOne_Sol_Case5
	case 6, HeartSoul_PickOne_Sol_Case6
	case 7, HeartSoul_PickOne_Sol_Case7
	case 8, HeartSoul_PickOne_Sol_Case8
	case 9, HeartSoul_PickOne_Sol_Case9
	case 10, HeartSoul_PickOne_Sol_Case10
	case 11, HeartSoul_PickOne_Sol_Case11
	case 12, HeartSoul_PickOne_Sol_Case12
	case 13, HeartSoul_PickOne_Sol_Case13
	case 14, HeartSoul_PickOne_Sol_Case14
	case 15, HeartSoul_PickOne_Sol_Case15
	case 16, HeartSoul_PickOne_Sol_Case16
	case 17, HeartSoul_PickOne_Sol_Case17
	case 18, HeartSoul_PickOne_Sol_Case18
	case 19, HeartSoul_PickOne_Sol_Case19
	goto HeartSoul_EventScript_PickOne_Sol
	end

HeartSoul_PickOne_Sol_Case0:
	setvar VAR_TEMP_SPECIES, SPECIES_DUGTRIO
	return
HeartSoul_PickOne_Sol_Case1:
	setvar VAR_TEMP_SPECIES, SPECIES_SANDSLASH
	return
HeartSoul_PickOne_Sol_Case2:
	setvar VAR_TEMP_SPECIES, SPECIES_RHYDON
	return
HeartSoul_PickOne_Sol_Case3:
	setvar VAR_TEMP_SPECIES, SPECIES_GOLEM
	return
HeartSoul_PickOne_Sol_Case4:
	setvar VAR_TEMP_SPECIES, SPECIES_MAROWAK
	return
HeartSoul_PickOne_Sol_Case5:
	setvar VAR_TEMP_SPECIES, SPECIES_DONPHAN
	return
HeartSoul_PickOne_Sol_Case6:
	setvar VAR_TEMP_SPECIES, SPECIES_FLYGON
	return
HeartSoul_PickOne_Sol_Case7:
	setvar VAR_TEMP_SPECIES, SPECIES_GARCHOMP
	return
HeartSoul_PickOne_Sol_Case8:
	setvar VAR_TEMP_SPECIES, SPECIES_EXCADRILL
	return
HeartSoul_PickOne_Sol_Case9:
	setvar VAR_TEMP_SPECIES, SPECIES_KROOKODILE
	return
HeartSoul_PickOne_Sol_Case10:
	setvar VAR_TEMP_SPECIES, SPECIES_GLISCOR
	return
HeartSoul_PickOne_Sol_Case11:
	setvar VAR_TEMP_SPECIES, SPECIES_CAMERUPT
	return
HeartSoul_PickOne_Sol_Case12:
	setvar VAR_TEMP_SPECIES, SPECIES_TORTERRA
	return
HeartSoul_PickOne_Sol_Case13:
	setvar VAR_TEMP_SPECIES, SPECIES_HIPPOWDON
	return
HeartSoul_PickOne_Sol_Case14:
	setvar VAR_TEMP_SPECIES, SPECIES_CLAYDOL
	return
HeartSoul_PickOne_Sol_Case15:
	setvar VAR_TEMP_SPECIES, SPECIES_GASTRODON
	return
HeartSoul_PickOne_Sol_Case16:
	setvar VAR_TEMP_SPECIES, SPECIES_STUNFISK
	return
HeartSoul_PickOne_Sol_Case17:
	setvar VAR_TEMP_SPECIES, SPECIES_NIDOKING
	return
HeartSoul_PickOne_Sol_Case18:
	setvar VAR_TEMP_SPECIES, SPECIES_STEELIX
	return
HeartSoul_PickOne_Sol_Case19:
	setvar VAR_TEMP_SPECIES, SPECIES_WHISCASH
	return

HeartSoul_Text_Mon_Sol_0:
	.string "TRIOPIKEUR$"
HeartSoul_Text_Mon_Sol_1:
	.string "SABLAIREAU$"
HeartSoul_Text_Mon_Sol_2:
	.string "RHINOFÉROS$"
HeartSoul_Text_Mon_Sol_3:
	.string "GROLEM$"
HeartSoul_Text_Mon_Sol_4:
	.string "OSSATUEUR$"
HeartSoul_Text_Mon_Sol_5:
	.string "DONPHAN$"
HeartSoul_Text_Mon_Sol_6:
	.string "LIBÉGON$"
HeartSoul_Text_Mon_Sol_7:
	.string "CARCHACROK$"
HeartSoul_Text_Mon_Sol_8:
	.string "MINOTAUPE$"
HeartSoul_Text_Mon_Sol_9:
	.string "CROCORIBLE$"
HeartSoul_Text_Mon_Sol_10:
	.string "SCORVOL$"
HeartSoul_Text_Mon_Sol_11:
	.string "CAMÉRUPT$"
HeartSoul_Text_Mon_Sol_12:
	.string "TORTERRA$"
HeartSoul_Text_Mon_Sol_13:
	.string "HIPPODOCUS$"
HeartSoul_Text_Mon_Sol_14:
	.string "KAORINE$"
HeartSoul_Text_Mon_Sol_15:
	.string "TRITOSOR$"
HeartSoul_Text_Mon_Sol_16:
	.string "LIMONDE$"
HeartSoul_Text_Mon_Sol_17:
	.string "NIDOKING$"
HeartSoul_Text_Mon_Sol_18:
	.string "STEELIX$"
HeartSoul_Text_Mon_Sol_19:
	.string "BARBICHA$"

HeartSoul_EventScript_ChooseTeam_Sol::
	call HeartSoul_EventScript_PickOne_Sol
	copyvar VAR_0x8004, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Sol_Slot2::
	call HeartSoul_EventScript_PickOne_Sol
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Sol_Slot2
	copyvar VAR_0x8005, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Sol_Slot3::
	call HeartSoul_EventScript_PickOne_Sol
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Sol_Slot3
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Sol_Slot3
	copyvar VAR_0x8006, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Sol_Slot4::
	call HeartSoul_EventScript_PickOne_Sol
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Sol_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Sol_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8006
	goto_if_eq HeartSoul_ChooseTeam_Sol_Slot4
	copyvar VAR_0x8007, VAR_TEMP_SPECIES
	givemon VAR_0x8004, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8005, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8006, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8007, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	setflag FLAG_SYS_POKEMON_GET
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox HeartSoul_Text_ChoisisAttaquesIntro, MSGBOX_DEFAULT
HeartSoul_ChooseMoves_Sol_ChooseMon::
	msgbox HeartSoul_Text_ChoisirPokemonAttaque, MSGBOX_DEFAULT
	setmoverelearnerstate MOVE_RELEARNER_LEVEL_UP_MOVES
	chooseboxmon SELECT_PC_MON_MOVE_RELEARNER
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartSoul_EventScript_CinnabarAttack
	special HasMovesToRelearn
	goto_if_eq VAR_RESULT, FALSE, HeartSoul_ChooseMoves_Sol_NoMove
	msgbox HeartSoul_Text_QuelleAttaque, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto HeartSoul_ChooseMoves_Sol_ChooseMon
HeartSoul_ChooseMoves_Sol_NoMove:
	msgbox HeartSoul_Text_PlusRienAApprendre, MSGBOX_DEFAULT
	goto HeartSoul_ChooseMoves_Sol_ChooseMon
	end

HeartSoul_EventScript_PickOne_Vol::
	dynmultipush HeartSoul_Text_Mon_Vol_0, 0
	dynmultipush HeartSoul_Text_Mon_Vol_1, 1
	dynmultipush HeartSoul_Text_Mon_Vol_2, 2
	dynmultipush HeartSoul_Text_Mon_Vol_3, 3
	dynmultipush HeartSoul_Text_Mon_Vol_4, 4
	dynmultipush HeartSoul_Text_Mon_Vol_5, 5
	dynmultipush HeartSoul_Text_Mon_Vol_6, 6
	dynmultipush HeartSoul_Text_Mon_Vol_7, 7
	dynmultipush HeartSoul_Text_Mon_Vol_8, 8
	dynmultipush HeartSoul_Text_Mon_Vol_9, 9
	dynmultipush HeartSoul_Text_Mon_Vol_10, 10
	dynmultipush HeartSoul_Text_Mon_Vol_11, 11
	dynmultipush HeartSoul_Text_Mon_Vol_12, 12
	dynmultipush HeartSoul_Text_Mon_Vol_13, 13
	dynmultipush HeartSoul_Text_Mon_Vol_14, 14
	dynmultipush HeartSoul_Text_Mon_Vol_15, 15
	dynmultipush HeartSoul_Text_Mon_Vol_16, 16
	dynmultipush HeartSoul_Text_Mon_Vol_17, 17
	dynmultipush HeartSoul_Text_Mon_Vol_18, 18
	dynmultipush HeartSoul_Text_Mon_Vol_19, 19
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_PickOne_Vol
	switch VAR_RESULT
	case 0, HeartSoul_PickOne_Vol_Case0
	case 1, HeartSoul_PickOne_Vol_Case1
	case 2, HeartSoul_PickOne_Vol_Case2
	case 3, HeartSoul_PickOne_Vol_Case3
	case 4, HeartSoul_PickOne_Vol_Case4
	case 5, HeartSoul_PickOne_Vol_Case5
	case 6, HeartSoul_PickOne_Vol_Case6
	case 7, HeartSoul_PickOne_Vol_Case7
	case 8, HeartSoul_PickOne_Vol_Case8
	case 9, HeartSoul_PickOne_Vol_Case9
	case 10, HeartSoul_PickOne_Vol_Case10
	case 11, HeartSoul_PickOne_Vol_Case11
	case 12, HeartSoul_PickOne_Vol_Case12
	case 13, HeartSoul_PickOne_Vol_Case13
	case 14, HeartSoul_PickOne_Vol_Case14
	case 15, HeartSoul_PickOne_Vol_Case15
	case 16, HeartSoul_PickOne_Vol_Case16
	case 17, HeartSoul_PickOne_Vol_Case17
	case 18, HeartSoul_PickOne_Vol_Case18
	case 19, HeartSoul_PickOne_Vol_Case19
	goto HeartSoul_EventScript_PickOne_Vol
	end

HeartSoul_PickOne_Vol_Case0:
	setvar VAR_TEMP_SPECIES, SPECIES_PIDGEOT
	return
HeartSoul_PickOne_Vol_Case1:
	setvar VAR_TEMP_SPECIES, SPECIES_FEAROW
	return
HeartSoul_PickOne_Vol_Case2:
	setvar VAR_TEMP_SPECIES, SPECIES_AERODACTYL
	return
HeartSoul_PickOne_Vol_Case3:
	setvar VAR_TEMP_SPECIES, SPECIES_CHARIZARD
	return
HeartSoul_PickOne_Vol_Case4:
	setvar VAR_TEMP_SPECIES, SPECIES_DODRIO
	return
HeartSoul_PickOne_Vol_Case5:
	setvar VAR_TEMP_SPECIES, SPECIES_SKARMORY
	return
HeartSoul_PickOne_Vol_Case6:
	setvar VAR_TEMP_SPECIES, SPECIES_CROBAT
	return
HeartSoul_PickOne_Vol_Case7:
	setvar VAR_TEMP_SPECIES, SPECIES_TOGEKISS
	return
HeartSoul_PickOne_Vol_Case8:
	setvar VAR_TEMP_SPECIES, SPECIES_STARAPTOR
	return
HeartSoul_PickOne_Vol_Case9:
	setvar VAR_TEMP_SPECIES, SPECIES_HONCHKROW
	return
HeartSoul_PickOne_Vol_Case10:
	setvar VAR_TEMP_SPECIES, SPECIES_SWELLOW
	return
HeartSoul_PickOne_Vol_Case11:
	setvar VAR_TEMP_SPECIES, SPECIES_NOIVERN
	return
HeartSoul_PickOne_Vol_Case12:
	setvar VAR_TEMP_SPECIES, SPECIES_TALONFLAME
	return
HeartSoul_PickOne_Vol_Case13:
	setvar VAR_TEMP_SPECIES, SPECIES_BRAVIARY
	return
HeartSoul_PickOne_Vol_Case14:
	setvar VAR_TEMP_SPECIES, SPECIES_MANDIBUZZ
	return
HeartSoul_PickOne_Vol_Case15:
	setvar VAR_TEMP_SPECIES, SPECIES_CORVIKNIGHT
	return
HeartSoul_PickOne_Vol_Case16:
	setvar VAR_TEMP_SPECIES, SPECIES_TORNADUS
	return
HeartSoul_PickOne_Vol_Case17:
	setvar VAR_TEMP_SPECIES, SPECIES_GYARADOS
	return
HeartSoul_PickOne_Vol_Case18:
	setvar VAR_TEMP_SPECIES, SPECIES_SALAMENCE
	return
HeartSoul_PickOne_Vol_Case19:
	setvar VAR_TEMP_SPECIES, SPECIES_ALTARIA
	return

HeartSoul_Text_Mon_Vol_0:
	.string "ROUCARNAGE$"
HeartSoul_Text_Mon_Vol_1:
	.string "RAPASDEPIC$"
HeartSoul_Text_Mon_Vol_2:
	.string "PTÉRA$"
HeartSoul_Text_Mon_Vol_3:
	.string "DRACAUFEU$"
HeartSoul_Text_Mon_Vol_4:
	.string "DODRIO$"
HeartSoul_Text_Mon_Vol_5:
	.string "AIRMURE$"
HeartSoul_Text_Mon_Vol_6:
	.string "NOSTENFER$"
HeartSoul_Text_Mon_Vol_7:
	.string "TOGEKISS$"
HeartSoul_Text_Mon_Vol_8:
	.string "ÉTOURAPTOR$"
HeartSoul_Text_Mon_Vol_9:
	.string "CORBOSS$"
HeartSoul_Text_Mon_Vol_10:
	.string "HÉLÉDELLE$"
HeartSoul_Text_Mon_Vol_11:
	.string "BRUYVERNE$"
HeartSoul_Text_Mon_Vol_12:
	.string "FLAMBUSARD$"
HeartSoul_Text_Mon_Vol_13:
	.string "GUERIAIGLE$"
HeartSoul_Text_Mon_Vol_14:
	.string "VAUTUTRICE$"
HeartSoul_Text_Mon_Vol_15:
	.string "CORVAILLUS$"
HeartSoul_Text_Mon_Vol_16:
	.string "BORÉAS$"
HeartSoul_Text_Mon_Vol_17:
	.string "LÉVIATOR$"
HeartSoul_Text_Mon_Vol_18:
	.string "DRATTAK$"
HeartSoul_Text_Mon_Vol_19:
	.string "ALTARIA$"

HeartSoul_EventScript_ChooseTeam_Vol::
	call HeartSoul_EventScript_PickOne_Vol
	copyvar VAR_0x8004, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Vol_Slot2::
	call HeartSoul_EventScript_PickOne_Vol
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Vol_Slot2
	copyvar VAR_0x8005, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Vol_Slot3::
	call HeartSoul_EventScript_PickOne_Vol
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Vol_Slot3
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Vol_Slot3
	copyvar VAR_0x8006, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Vol_Slot4::
	call HeartSoul_EventScript_PickOne_Vol
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Vol_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Vol_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8006
	goto_if_eq HeartSoul_ChooseTeam_Vol_Slot4
	copyvar VAR_0x8007, VAR_TEMP_SPECIES
	givemon VAR_0x8004, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8005, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8006, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8007, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	setflag FLAG_SYS_POKEMON_GET
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox HeartSoul_Text_ChoisisAttaquesIntro, MSGBOX_DEFAULT
HeartSoul_ChooseMoves_Vol_ChooseMon::
	msgbox HeartSoul_Text_ChoisirPokemonAttaque, MSGBOX_DEFAULT
	setmoverelearnerstate MOVE_RELEARNER_LEVEL_UP_MOVES
	chooseboxmon SELECT_PC_MON_MOVE_RELEARNER
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartSoul_EventScript_CinnabarAttack
	special HasMovesToRelearn
	goto_if_eq VAR_RESULT, FALSE, HeartSoul_ChooseMoves_Vol_NoMove
	msgbox HeartSoul_Text_QuelleAttaque, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto HeartSoul_ChooseMoves_Vol_ChooseMon
HeartSoul_ChooseMoves_Vol_NoMove:
	msgbox HeartSoul_Text_PlusRienAApprendre, MSGBOX_DEFAULT
	goto HeartSoul_ChooseMoves_Vol_ChooseMon
	end

HeartSoul_EventScript_PickOne_Psy::
	dynmultipush HeartSoul_Text_Mon_Psy_0, 0
	dynmultipush HeartSoul_Text_Mon_Psy_1, 1
	dynmultipush HeartSoul_Text_Mon_Psy_2, 2
	dynmultipush HeartSoul_Text_Mon_Psy_3, 3
	dynmultipush HeartSoul_Text_Mon_Psy_4, 4
	dynmultipush HeartSoul_Text_Mon_Psy_5, 5
	dynmultipush HeartSoul_Text_Mon_Psy_6, 6
	dynmultipush HeartSoul_Text_Mon_Psy_7, 7
	dynmultipush HeartSoul_Text_Mon_Psy_8, 8
	dynmultipush HeartSoul_Text_Mon_Psy_9, 9
	dynmultipush HeartSoul_Text_Mon_Psy_10, 10
	dynmultipush HeartSoul_Text_Mon_Psy_11, 11
	dynmultipush HeartSoul_Text_Mon_Psy_12, 12
	dynmultipush HeartSoul_Text_Mon_Psy_13, 13
	dynmultipush HeartSoul_Text_Mon_Psy_14, 14
	dynmultipush HeartSoul_Text_Mon_Psy_15, 15
	dynmultipush HeartSoul_Text_Mon_Psy_16, 16
	dynmultipush HeartSoul_Text_Mon_Psy_17, 17
	dynmultipush HeartSoul_Text_Mon_Psy_18, 18
	dynmultipush HeartSoul_Text_Mon_Psy_19, 19
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_PickOne_Psy
	switch VAR_RESULT
	case 0, HeartSoul_PickOne_Psy_Case0
	case 1, HeartSoul_PickOne_Psy_Case1
	case 2, HeartSoul_PickOne_Psy_Case2
	case 3, HeartSoul_PickOne_Psy_Case3
	case 4, HeartSoul_PickOne_Psy_Case4
	case 5, HeartSoul_PickOne_Psy_Case5
	case 6, HeartSoul_PickOne_Psy_Case6
	case 7, HeartSoul_PickOne_Psy_Case7
	case 8, HeartSoul_PickOne_Psy_Case8
	case 9, HeartSoul_PickOne_Psy_Case9
	case 10, HeartSoul_PickOne_Psy_Case10
	case 11, HeartSoul_PickOne_Psy_Case11
	case 12, HeartSoul_PickOne_Psy_Case12
	case 13, HeartSoul_PickOne_Psy_Case13
	case 14, HeartSoul_PickOne_Psy_Case14
	case 15, HeartSoul_PickOne_Psy_Case15
	case 16, HeartSoul_PickOne_Psy_Case16
	case 17, HeartSoul_PickOne_Psy_Case17
	case 18, HeartSoul_PickOne_Psy_Case18
	case 19, HeartSoul_PickOne_Psy_Case19
	goto HeartSoul_EventScript_PickOne_Psy
	end

HeartSoul_PickOne_Psy_Case0:
	setvar VAR_TEMP_SPECIES, SPECIES_ALAKAZAM
	return
HeartSoul_PickOne_Psy_Case1:
	setvar VAR_TEMP_SPECIES, SPECIES_SLOWBRO
	return
HeartSoul_PickOne_Psy_Case2:
	setvar VAR_TEMP_SPECIES, SPECIES_STARMIE
	return
HeartSoul_PickOne_Psy_Case3:
	setvar VAR_TEMP_SPECIES, SPECIES_MR_MIME
	return
HeartSoul_PickOne_Psy_Case4:
	setvar VAR_TEMP_SPECIES, SPECIES_EXEGGUTOR
	return
HeartSoul_PickOne_Psy_Case5:
	setvar VAR_TEMP_SPECIES, SPECIES_GARDEVOIR
	return
HeartSoul_PickOne_Psy_Case6:
	setvar VAR_TEMP_SPECIES, SPECIES_GALLADE
	return
HeartSoul_PickOne_Psy_Case7:
	setvar VAR_TEMP_SPECIES, SPECIES_METAGROSS
	return
HeartSoul_PickOne_Psy_Case8:
	setvar VAR_TEMP_SPECIES, SPECIES_REUNICLUS
	return
HeartSoul_PickOne_Psy_Case9:
	setvar VAR_TEMP_SPECIES, SPECIES_BRONZONG
	return
HeartSoul_PickOne_Psy_Case10:
	setvar VAR_TEMP_SPECIES, SPECIES_SIGILYPH
	return
HeartSoul_PickOne_Psy_Case11:
	setvar VAR_TEMP_SPECIES, SPECIES_MUSHARNA
	return
HeartSoul_PickOne_Psy_Case12:
	setvar VAR_TEMP_SPECIES, SPECIES_ESPEON
	return
HeartSoul_PickOne_Psy_Case13:
	setvar VAR_TEMP_SPECIES, SPECIES_XATU
	return
HeartSoul_PickOne_Psy_Case14:
	setvar VAR_TEMP_SPECIES, SPECIES_SLOWKING
	return
HeartSoul_PickOne_Psy_Case15:
	setvar VAR_TEMP_SPECIES, SPECIES_MALAMAR
	return
HeartSoul_PickOne_Psy_Case16:
	setvar VAR_TEMP_SPECIES, SPECIES_ORANGURU
	return
HeartSoul_PickOne_Psy_Case17:
	setvar VAR_TEMP_SPECIES, SPECIES_GIRAFARIG
	return
HeartSoul_PickOne_Psy_Case18:
	setvar VAR_TEMP_SPECIES, SPECIES_WOBBUFFET
	return
HeartSoul_PickOne_Psy_Case19:
	setvar VAR_TEMP_SPECIES, SPECIES_GRUMPIG
	return

HeartSoul_Text_Mon_Psy_0:
	.string "ALAKAZAM$"
HeartSoul_Text_Mon_Psy_1:
	.string "FLAGADOSS$"
HeartSoul_Text_Mon_Psy_2:
	.string "STAROSS$"
HeartSoul_Text_Mon_Psy_3:
	.string "M. MIME$"
HeartSoul_Text_Mon_Psy_4:
	.string "NOADKOKO$"
HeartSoul_Text_Mon_Psy_5:
	.string "GARDEVOIR$"
HeartSoul_Text_Mon_Psy_6:
	.string "GALLAME$"
HeartSoul_Text_Mon_Psy_7:
	.string "METALOSSE$"
HeartSoul_Text_Mon_Psy_8:
	.string "SYMBIOS$"
HeartSoul_Text_Mon_Psy_9:
	.string "ARCHEODONG$"
HeartSoul_Text_Mon_Psy_10:
	.string "CRYPTÉRO$"
HeartSoul_Text_Mon_Psy_11:
	.string "MUSHANA$"
HeartSoul_Text_Mon_Psy_12:
	.string "MENTALI$"
HeartSoul_Text_Mon_Psy_13:
	.string "XATU$"
HeartSoul_Text_Mon_Psy_14:
	.string "ROIGADA$"
HeartSoul_Text_Mon_Psy_15:
	.string "SEPIATROCE$"
HeartSoul_Text_Mon_Psy_16:
	.string "GOUROUTAN$"
HeartSoul_Text_Mon_Psy_17:
	.string "GIRAFARIG$"
HeartSoul_Text_Mon_Psy_18:
	.string "QULBUTOKÉ$"
HeartSoul_Text_Mon_Psy_19:
	.string "GRORET$"

HeartSoul_EventScript_ChooseTeam_Psy::
	call HeartSoul_EventScript_PickOne_Psy
	copyvar VAR_0x8004, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Psy_Slot2::
	call HeartSoul_EventScript_PickOne_Psy
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Psy_Slot2
	copyvar VAR_0x8005, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Psy_Slot3::
	call HeartSoul_EventScript_PickOne_Psy
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Psy_Slot3
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Psy_Slot3
	copyvar VAR_0x8006, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Psy_Slot4::
	call HeartSoul_EventScript_PickOne_Psy
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Psy_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Psy_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8006
	goto_if_eq HeartSoul_ChooseTeam_Psy_Slot4
	copyvar VAR_0x8007, VAR_TEMP_SPECIES
	givemon VAR_0x8004, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8005, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8006, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8007, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	setflag FLAG_SYS_POKEMON_GET
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox HeartSoul_Text_ChoisisAttaquesIntro, MSGBOX_DEFAULT
HeartSoul_ChooseMoves_Psy_ChooseMon::
	msgbox HeartSoul_Text_ChoisirPokemonAttaque, MSGBOX_DEFAULT
	setmoverelearnerstate MOVE_RELEARNER_LEVEL_UP_MOVES
	chooseboxmon SELECT_PC_MON_MOVE_RELEARNER
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartSoul_EventScript_CinnabarAttack
	special HasMovesToRelearn
	goto_if_eq VAR_RESULT, FALSE, HeartSoul_ChooseMoves_Psy_NoMove
	msgbox HeartSoul_Text_QuelleAttaque, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto HeartSoul_ChooseMoves_Psy_ChooseMon
HeartSoul_ChooseMoves_Psy_NoMove:
	msgbox HeartSoul_Text_PlusRienAApprendre, MSGBOX_DEFAULT
	goto HeartSoul_ChooseMoves_Psy_ChooseMon
	end

HeartSoul_EventScript_PickOne_Insecte::
	dynmultipush HeartSoul_Text_Mon_Insecte_0, 0
	dynmultipush HeartSoul_Text_Mon_Insecte_1, 1
	dynmultipush HeartSoul_Text_Mon_Insecte_2, 2
	dynmultipush HeartSoul_Text_Mon_Insecte_3, 3
	dynmultipush HeartSoul_Text_Mon_Insecte_4, 4
	dynmultipush HeartSoul_Text_Mon_Insecte_5, 5
	dynmultipush HeartSoul_Text_Mon_Insecte_6, 6
	dynmultipush HeartSoul_Text_Mon_Insecte_7, 7
	dynmultipush HeartSoul_Text_Mon_Insecte_8, 8
	dynmultipush HeartSoul_Text_Mon_Insecte_9, 9
	dynmultipush HeartSoul_Text_Mon_Insecte_10, 10
	dynmultipush HeartSoul_Text_Mon_Insecte_11, 11
	dynmultipush HeartSoul_Text_Mon_Insecte_12, 12
	dynmultipush HeartSoul_Text_Mon_Insecte_13, 13
	dynmultipush HeartSoul_Text_Mon_Insecte_14, 14
	dynmultipush HeartSoul_Text_Mon_Insecte_15, 15
	dynmultipush HeartSoul_Text_Mon_Insecte_16, 16
	dynmultipush HeartSoul_Text_Mon_Insecte_17, 17
	dynmultipush HeartSoul_Text_Mon_Insecte_18, 18
	dynmultipush HeartSoul_Text_Mon_Insecte_19, 19
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_PickOne_Insecte
	switch VAR_RESULT
	case 0, HeartSoul_PickOne_Insecte_Case0
	case 1, HeartSoul_PickOne_Insecte_Case1
	case 2, HeartSoul_PickOne_Insecte_Case2
	case 3, HeartSoul_PickOne_Insecte_Case3
	case 4, HeartSoul_PickOne_Insecte_Case4
	case 5, HeartSoul_PickOne_Insecte_Case5
	case 6, HeartSoul_PickOne_Insecte_Case6
	case 7, HeartSoul_PickOne_Insecte_Case7
	case 8, HeartSoul_PickOne_Insecte_Case8
	case 9, HeartSoul_PickOne_Insecte_Case9
	case 10, HeartSoul_PickOne_Insecte_Case10
	case 11, HeartSoul_PickOne_Insecte_Case11
	case 12, HeartSoul_PickOne_Insecte_Case12
	case 13, HeartSoul_PickOne_Insecte_Case13
	case 14, HeartSoul_PickOne_Insecte_Case14
	case 15, HeartSoul_PickOne_Insecte_Case15
	case 16, HeartSoul_PickOne_Insecte_Case16
	case 17, HeartSoul_PickOne_Insecte_Case17
	case 18, HeartSoul_PickOne_Insecte_Case18
	case 19, HeartSoul_PickOne_Insecte_Case19
	goto HeartSoul_EventScript_PickOne_Insecte
	end

HeartSoul_PickOne_Insecte_Case0:
	setvar VAR_TEMP_SPECIES, SPECIES_BUTTERFREE
	return
HeartSoul_PickOne_Insecte_Case1:
	setvar VAR_TEMP_SPECIES, SPECIES_BEEDRILL
	return
HeartSoul_PickOne_Insecte_Case2:
	setvar VAR_TEMP_SPECIES, SPECIES_SCIZOR
	return
HeartSoul_PickOne_Insecte_Case3:
	setvar VAR_TEMP_SPECIES, SPECIES_PINSIR
	return
HeartSoul_PickOne_Insecte_Case4:
	setvar VAR_TEMP_SPECIES, SPECIES_HERACROSS
	return
HeartSoul_PickOne_Insecte_Case5:
	setvar VAR_TEMP_SPECIES, SPECIES_VENOMOTH
	return
HeartSoul_PickOne_Insecte_Case6:
	setvar VAR_TEMP_SPECIES, SPECIES_VOLCARONA
	return
HeartSoul_PickOne_Insecte_Case7:
	setvar VAR_TEMP_SPECIES, SPECIES_GALVANTULA
	return
HeartSoul_PickOne_Insecte_Case8:
	setvar VAR_TEMP_SPECIES, SPECIES_VIKAVOLT
	return
HeartSoul_PickOne_Insecte_Case9:
	setvar VAR_TEMP_SPECIES, SPECIES_YANMEGA
	return
HeartSoul_PickOne_Insecte_Case10:
	setvar VAR_TEMP_SPECIES, SPECIES_NINJASK
	return
HeartSoul_PickOne_Insecte_Case11:
	setvar VAR_TEMP_SPECIES, SPECIES_SHEDINJA
	return
HeartSoul_PickOne_Insecte_Case12:
	setvar VAR_TEMP_SPECIES, SPECIES_VESPIQUEN
	return
HeartSoul_PickOne_Insecte_Case13:
	setvar VAR_TEMP_SPECIES, SPECIES_CRUSTLE
	return
HeartSoul_PickOne_Insecte_Case14:
	setvar VAR_TEMP_SPECIES, SPECIES_ESCAVALIER
	return
HeartSoul_PickOne_Insecte_Case15:
	setvar VAR_TEMP_SPECIES, SPECIES_ACCELGOR
	return
HeartSoul_PickOne_Insecte_Case16:
	setvar VAR_TEMP_SPECIES, SPECIES_LEAVANNY
	return
HeartSoul_PickOne_Insecte_Case17:
	setvar VAR_TEMP_SPECIES, SPECIES_FORRETRESS
	return
HeartSoul_PickOne_Insecte_Case18:
	setvar VAR_TEMP_SPECIES, SPECIES_ARMALDO
	return
HeartSoul_PickOne_Insecte_Case19:
	setvar VAR_TEMP_SPECIES, SPECIES_VOLBEAT
	return

HeartSoul_Text_Mon_Insecte_0:
	.string "PAPILUSION$"
HeartSoul_Text_Mon_Insecte_1:
	.string "DARDARGNAN$"
HeartSoul_Text_Mon_Insecte_2:
	.string "CIZAYOX$"
HeartSoul_Text_Mon_Insecte_3:
	.string "SCARABRUTE$"
HeartSoul_Text_Mon_Insecte_4:
	.string "SCARHINO$"
HeartSoul_Text_Mon_Insecte_5:
	.string "AÉROMITE$"
HeartSoul_Text_Mon_Insecte_6:
	.string "PYRAX$"
HeartSoul_Text_Mon_Insecte_7:
	.string "MYGAVOLT$"
HeartSoul_Text_Mon_Insecte_8:
	.string "LUCANON$"
HeartSoul_Text_Mon_Insecte_9:
	.string "YANMEGA$"
HeartSoul_Text_Mon_Insecte_10:
	.string "NINJASK$"
HeartSoul_Text_Mon_Insecte_11:
	.string "MUNJA$"
HeartSoul_Text_Mon_Insecte_12:
	.string "APIREINE$"
HeartSoul_Text_Mon_Insecte_13:
	.string "CRABARAQUE$"
HeartSoul_Text_Mon_Insecte_14:
	.string "LANÇARGOT$"
HeartSoul_Text_Mon_Insecte_15:
	.string "LIMASPEED$"
HeartSoul_Text_Mon_Insecte_16:
	.string "MANTERNEL$"
HeartSoul_Text_Mon_Insecte_17:
	.string "FORETRESS$"
HeartSoul_Text_Mon_Insecte_18:
	.string "ARMALDO$"
HeartSoul_Text_Mon_Insecte_19:
	.string "MUCIOLE$"

HeartSoul_EventScript_ChooseTeam_Insecte::
	call HeartSoul_EventScript_PickOne_Insecte
	copyvar VAR_0x8004, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Insecte_Slot2::
	call HeartSoul_EventScript_PickOne_Insecte
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Insecte_Slot2
	copyvar VAR_0x8005, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Insecte_Slot3::
	call HeartSoul_EventScript_PickOne_Insecte
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Insecte_Slot3
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Insecte_Slot3
	copyvar VAR_0x8006, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Insecte_Slot4::
	call HeartSoul_EventScript_PickOne_Insecte
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Insecte_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Insecte_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8006
	goto_if_eq HeartSoul_ChooseTeam_Insecte_Slot4
	copyvar VAR_0x8007, VAR_TEMP_SPECIES
	givemon VAR_0x8004, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8005, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8006, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8007, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	setflag FLAG_SYS_POKEMON_GET
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox HeartSoul_Text_ChoisisAttaquesIntro, MSGBOX_DEFAULT
HeartSoul_ChooseMoves_Insecte_ChooseMon::
	msgbox HeartSoul_Text_ChoisirPokemonAttaque, MSGBOX_DEFAULT
	setmoverelearnerstate MOVE_RELEARNER_LEVEL_UP_MOVES
	chooseboxmon SELECT_PC_MON_MOVE_RELEARNER
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartSoul_EventScript_CinnabarAttack
	special HasMovesToRelearn
	goto_if_eq VAR_RESULT, FALSE, HeartSoul_ChooseMoves_Insecte_NoMove
	msgbox HeartSoul_Text_QuelleAttaque, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto HeartSoul_ChooseMoves_Insecte_ChooseMon
HeartSoul_ChooseMoves_Insecte_NoMove:
	msgbox HeartSoul_Text_PlusRienAApprendre, MSGBOX_DEFAULT
	goto HeartSoul_ChooseMoves_Insecte_ChooseMon
	end

HeartSoul_EventScript_PickOne_Roche::
	dynmultipush HeartSoul_Text_Mon_Roche_0, 0
	dynmultipush HeartSoul_Text_Mon_Roche_1, 1
	dynmultipush HeartSoul_Text_Mon_Roche_2, 2
	dynmultipush HeartSoul_Text_Mon_Roche_3, 3
	dynmultipush HeartSoul_Text_Mon_Roche_4, 4
	dynmultipush HeartSoul_Text_Mon_Roche_5, 5
	dynmultipush HeartSoul_Text_Mon_Roche_6, 6
	dynmultipush HeartSoul_Text_Mon_Roche_7, 7
	dynmultipush HeartSoul_Text_Mon_Roche_8, 8
	dynmultipush HeartSoul_Text_Mon_Roche_9, 9
	dynmultipush HeartSoul_Text_Mon_Roche_10, 10
	dynmultipush HeartSoul_Text_Mon_Roche_11, 11
	dynmultipush HeartSoul_Text_Mon_Roche_12, 12
	dynmultipush HeartSoul_Text_Mon_Roche_13, 13
	dynmultipush HeartSoul_Text_Mon_Roche_14, 14
	dynmultipush HeartSoul_Text_Mon_Roche_15, 15
	dynmultipush HeartSoul_Text_Mon_Roche_16, 16
	dynmultipush HeartSoul_Text_Mon_Roche_17, 17
	dynmultipush HeartSoul_Text_Mon_Roche_18, 18
	dynmultipush HeartSoul_Text_Mon_Roche_19, 19
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_PickOne_Roche
	switch VAR_RESULT
	case 0, HeartSoul_PickOne_Roche_Case0
	case 1, HeartSoul_PickOne_Roche_Case1
	case 2, HeartSoul_PickOne_Roche_Case2
	case 3, HeartSoul_PickOne_Roche_Case3
	case 4, HeartSoul_PickOne_Roche_Case4
	case 5, HeartSoul_PickOne_Roche_Case5
	case 6, HeartSoul_PickOne_Roche_Case6
	case 7, HeartSoul_PickOne_Roche_Case7
	case 8, HeartSoul_PickOne_Roche_Case8
	case 9, HeartSoul_PickOne_Roche_Case9
	case 10, HeartSoul_PickOne_Roche_Case10
	case 11, HeartSoul_PickOne_Roche_Case11
	case 12, HeartSoul_PickOne_Roche_Case12
	case 13, HeartSoul_PickOne_Roche_Case13
	case 14, HeartSoul_PickOne_Roche_Case14
	case 15, HeartSoul_PickOne_Roche_Case15
	case 16, HeartSoul_PickOne_Roche_Case16
	case 17, HeartSoul_PickOne_Roche_Case17
	case 18, HeartSoul_PickOne_Roche_Case18
	case 19, HeartSoul_PickOne_Roche_Case19
	goto HeartSoul_EventScript_PickOne_Roche
	end

HeartSoul_PickOne_Roche_Case0:
	setvar VAR_TEMP_SPECIES, SPECIES_GOLEM
	return
HeartSoul_PickOne_Roche_Case1:
	setvar VAR_TEMP_SPECIES, SPECIES_ONIX
	return
HeartSoul_PickOne_Roche_Case2:
	setvar VAR_TEMP_SPECIES, SPECIES_RHYDON
	return
HeartSoul_PickOne_Roche_Case3:
	setvar VAR_TEMP_SPECIES, SPECIES_KABUTOPS
	return
HeartSoul_PickOne_Roche_Case4:
	setvar VAR_TEMP_SPECIES, SPECIES_OMASTAR
	return
HeartSoul_PickOne_Roche_Case5:
	setvar VAR_TEMP_SPECIES, SPECIES_AERODACTYL
	return
HeartSoul_PickOne_Roche_Case6:
	setvar VAR_TEMP_SPECIES, SPECIES_TYRANITAR
	return
HeartSoul_PickOne_Roche_Case7:
	setvar VAR_TEMP_SPECIES, SPECIES_AGGRON
	return
HeartSoul_PickOne_Roche_Case8:
	setvar VAR_TEMP_SPECIES, SPECIES_RAMPARDOS
	return
HeartSoul_PickOne_Roche_Case9:
	setvar VAR_TEMP_SPECIES, SPECIES_BASTIODON
	return
HeartSoul_PickOne_Roche_Case10:
	setvar VAR_TEMP_SPECIES, SPECIES_PROBOPASS
	return
HeartSoul_PickOne_Roche_Case11:
	setvar VAR_TEMP_SPECIES, SPECIES_GIGALITH
	return
HeartSoul_PickOne_Roche_Case12:
	setvar VAR_TEMP_SPECIES, SPECIES_CARBINK
	return
HeartSoul_PickOne_Roche_Case13:
	setvar VAR_TEMP_SPECIES, SPECIES_LYCANROC
	return
HeartSoul_PickOne_Roche_Case14:
	setvar VAR_TEMP_SPECIES, SPECIES_STONJOURNER
	return
HeartSoul_PickOne_Roche_Case15:
	setvar VAR_TEMP_SPECIES, SPECIES_COALOSSAL
	return
HeartSoul_PickOne_Roche_Case16:
	setvar VAR_TEMP_SPECIES, SPECIES_CRUSTLE
	return
HeartSoul_PickOne_Roche_Case17:
	setvar VAR_TEMP_SPECIES, SPECIES_CRADILY
	return
HeartSoul_PickOne_Roche_Case18:
	setvar VAR_TEMP_SPECIES, SPECIES_RELICANTH
	return
HeartSoul_PickOne_Roche_Case19:
	setvar VAR_TEMP_SPECIES, SPECIES_BARBARACLE
	return

HeartSoul_Text_Mon_Roche_0:
	.string "GROLEM$"
HeartSoul_Text_Mon_Roche_1:
	.string "ONIX$"
HeartSoul_Text_Mon_Roche_2:
	.string "RHINOFÉROS$"
HeartSoul_Text_Mon_Roche_3:
	.string "KABUTOPS$"
HeartSoul_Text_Mon_Roche_4:
	.string "AMONISTAR$"
HeartSoul_Text_Mon_Roche_5:
	.string "PTÉRA$"
HeartSoul_Text_Mon_Roche_6:
	.string "TYRANOCIF$"
HeartSoul_Text_Mon_Roche_7:
	.string "GALEKING$"
HeartSoul_Text_Mon_Roche_8:
	.string "CHARKOS$"
HeartSoul_Text_Mon_Roche_9:
	.string "BASTIODON$"
HeartSoul_Text_Mon_Roche_10:
	.string "TARINORME$"
HeartSoul_Text_Mon_Roche_11:
	.string "GIGALITHE$"
HeartSoul_Text_Mon_Roche_12:
	.string "STRASSIE$"
HeartSoul_Text_Mon_Roche_13:
	.string "LOUGAROC$"
HeartSoul_Text_Mon_Roche_14:
	.string "DOLMAN$"
HeartSoul_Text_Mon_Roche_15:
	.string "ANTHRAPODE$"
HeartSoul_Text_Mon_Roche_16:
	.string "CRABARAQUE$"
HeartSoul_Text_Mon_Roche_17:
	.string "VACILYS$"
HeartSoul_Text_Mon_Roche_18:
	.string "RELICANTH$"
HeartSoul_Text_Mon_Roche_19:
	.string "GOLGOPATHE$"

HeartSoul_EventScript_ChooseTeam_Roche::
	call HeartSoul_EventScript_PickOne_Roche
	copyvar VAR_0x8004, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Roche_Slot2::
	call HeartSoul_EventScript_PickOne_Roche
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Roche_Slot2
	copyvar VAR_0x8005, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Roche_Slot3::
	call HeartSoul_EventScript_PickOne_Roche
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Roche_Slot3
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Roche_Slot3
	copyvar VAR_0x8006, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Roche_Slot4::
	call HeartSoul_EventScript_PickOne_Roche
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Roche_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Roche_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8006
	goto_if_eq HeartSoul_ChooseTeam_Roche_Slot4
	copyvar VAR_0x8007, VAR_TEMP_SPECIES
	givemon VAR_0x8004, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8005, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8006, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8007, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	setflag FLAG_SYS_POKEMON_GET
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox HeartSoul_Text_ChoisisAttaquesIntro, MSGBOX_DEFAULT
HeartSoul_ChooseMoves_Roche_ChooseMon::
	msgbox HeartSoul_Text_ChoisirPokemonAttaque, MSGBOX_DEFAULT
	setmoverelearnerstate MOVE_RELEARNER_LEVEL_UP_MOVES
	chooseboxmon SELECT_PC_MON_MOVE_RELEARNER
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartSoul_EventScript_CinnabarAttack
	special HasMovesToRelearn
	goto_if_eq VAR_RESULT, FALSE, HeartSoul_ChooseMoves_Roche_NoMove
	msgbox HeartSoul_Text_QuelleAttaque, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto HeartSoul_ChooseMoves_Roche_ChooseMon
HeartSoul_ChooseMoves_Roche_NoMove:
	msgbox HeartSoul_Text_PlusRienAApprendre, MSGBOX_DEFAULT
	goto HeartSoul_ChooseMoves_Roche_ChooseMon
	end

HeartSoul_EventScript_PickOne_Spectre::
	dynmultipush HeartSoul_Text_Mon_Spectre_0, 0
	dynmultipush HeartSoul_Text_Mon_Spectre_1, 1
	dynmultipush HeartSoul_Text_Mon_Spectre_2, 2
	dynmultipush HeartSoul_Text_Mon_Spectre_3, 3
	dynmultipush HeartSoul_Text_Mon_Spectre_4, 4
	dynmultipush HeartSoul_Text_Mon_Spectre_5, 5
	dynmultipush HeartSoul_Text_Mon_Spectre_6, 6
	dynmultipush HeartSoul_Text_Mon_Spectre_7, 7
	dynmultipush HeartSoul_Text_Mon_Spectre_8, 8
	dynmultipush HeartSoul_Text_Mon_Spectre_9, 9
	dynmultipush HeartSoul_Text_Mon_Spectre_10, 10
	dynmultipush HeartSoul_Text_Mon_Spectre_11, 11
	dynmultipush HeartSoul_Text_Mon_Spectre_12, 12
	dynmultipush HeartSoul_Text_Mon_Spectre_13, 13
	dynmultipush HeartSoul_Text_Mon_Spectre_14, 14
	dynmultipush HeartSoul_Text_Mon_Spectre_15, 15
	dynmultipush HeartSoul_Text_Mon_Spectre_16, 16
	dynmultipush HeartSoul_Text_Mon_Spectre_17, 17
	dynmultipush HeartSoul_Text_Mon_Spectre_18, 18
	dynmultipush HeartSoul_Text_Mon_Spectre_19, 19
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_PickOne_Spectre
	switch VAR_RESULT
	case 0, HeartSoul_PickOne_Spectre_Case0
	case 1, HeartSoul_PickOne_Spectre_Case1
	case 2, HeartSoul_PickOne_Spectre_Case2
	case 3, HeartSoul_PickOne_Spectre_Case3
	case 4, HeartSoul_PickOne_Spectre_Case4
	case 5, HeartSoul_PickOne_Spectre_Case5
	case 6, HeartSoul_PickOne_Spectre_Case6
	case 7, HeartSoul_PickOne_Spectre_Case7
	case 8, HeartSoul_PickOne_Spectre_Case8
	case 9, HeartSoul_PickOne_Spectre_Case9
	case 10, HeartSoul_PickOne_Spectre_Case10
	case 11, HeartSoul_PickOne_Spectre_Case11
	case 12, HeartSoul_PickOne_Spectre_Case12
	case 13, HeartSoul_PickOne_Spectre_Case13
	case 14, HeartSoul_PickOne_Spectre_Case14
	case 15, HeartSoul_PickOne_Spectre_Case15
	case 16, HeartSoul_PickOne_Spectre_Case16
	case 17, HeartSoul_PickOne_Spectre_Case17
	case 18, HeartSoul_PickOne_Spectre_Case18
	case 19, HeartSoul_PickOne_Spectre_Case19
	goto HeartSoul_EventScript_PickOne_Spectre
	end

HeartSoul_PickOne_Spectre_Case0:
	setvar VAR_TEMP_SPECIES, SPECIES_GENGAR
	return
HeartSoul_PickOne_Spectre_Case1:
	setvar VAR_TEMP_SPECIES, SPECIES_MISMAGIUS
	return
HeartSoul_PickOne_Spectre_Case2:
	setvar VAR_TEMP_SPECIES, SPECIES_DUSKNOIR
	return
HeartSoul_PickOne_Spectre_Case3:
	setvar VAR_TEMP_SPECIES, SPECIES_DRIFBLIM
	return
HeartSoul_PickOne_Spectre_Case4:
	setvar VAR_TEMP_SPECIES, SPECIES_CHANDELURE
	return
HeartSoul_PickOne_Spectre_Case5:
	setvar VAR_TEMP_SPECIES, SPECIES_TREVENANT
	return
HeartSoul_PickOne_Spectre_Case6:
	setvar VAR_TEMP_SPECIES, SPECIES_AEGISLASH
	return
HeartSoul_PickOne_Spectre_Case7:
	setvar VAR_TEMP_SPECIES, SPECIES_SABLEYE
	return
HeartSoul_PickOne_Spectre_Case8:
	setvar VAR_TEMP_SPECIES, SPECIES_BANETTE
	return
HeartSoul_PickOne_Spectre_Case9:
	setvar VAR_TEMP_SPECIES, SPECIES_GOLURK
	return
HeartSoul_PickOne_Spectre_Case10:
	setvar VAR_TEMP_SPECIES, SPECIES_FROSLASS
	return
HeartSoul_PickOne_Spectre_Case11:
	setvar VAR_TEMP_SPECIES, SPECIES_COFAGRIGUS
	return
HeartSoul_PickOne_Spectre_Case12:
	setvar VAR_TEMP_SPECIES, SPECIES_POLTEAGEIST
	return
HeartSoul_PickOne_Spectre_Case13:
	setvar VAR_TEMP_SPECIES, SPECIES_RUNERIGUS
	return
HeartSoul_PickOne_Spectre_Case14:
	setvar VAR_TEMP_SPECIES, SPECIES_GOURGEIST
	return
HeartSoul_PickOne_Spectre_Case15:
	setvar VAR_TEMP_SPECIES, SPECIES_DHELMISE
	return
HeartSoul_PickOne_Spectre_Case16:
	setvar VAR_TEMP_SPECIES, SPECIES_PALOSSAND
	return
HeartSoul_PickOne_Spectre_Case17:
	setvar VAR_TEMP_SPECIES, SPECIES_SPIRITOMB
	return
HeartSoul_PickOne_Spectre_Case18:
	setvar VAR_TEMP_SPECIES, SPECIES_JELLICENT
	return
HeartSoul_PickOne_Spectre_Case19:
	setvar VAR_TEMP_SPECIES, SPECIES_MAROWAK_ALOLA
	return

HeartSoul_Text_Mon_Spectre_0:
	.string "ECTOPLASMA$"
HeartSoul_Text_Mon_Spectre_1:
	.string "MAGIRÊVE$"
HeartSoul_Text_Mon_Spectre_2:
	.string "NOCTUNOIR$"
HeartSoul_Text_Mon_Spectre_3:
	.string "GRODRIVE$"
HeartSoul_Text_Mon_Spectre_4:
	.string "LUGULABRE$"
HeartSoul_Text_Mon_Spectre_5:
	.string "DESSÉLIANDE$"
HeartSoul_Text_Mon_Spectre_6:
	.string "EXAGIDE$"
HeartSoul_Text_Mon_Spectre_7:
	.string "TÉNÉFIX$"
HeartSoul_Text_Mon_Spectre_8:
	.string "BRANETTE$"
HeartSoul_Text_Mon_Spectre_9:
	.string "GOLEMASTOC$"
HeartSoul_Text_Mon_Spectre_10:
	.string "MOMARTIK$"
HeartSoul_Text_Mon_Spectre_11:
	.string "TUTANKAFER$"
HeartSoul_Text_Mon_Spectre_12:
	.string "POLTHÉGEIST$"
HeartSoul_Text_Mon_Spectre_13:
	.string "TUTÉTÉKRI$"
HeartSoul_Text_Mon_Spectre_14:
	.string "BANSHITROUYE$"
HeartSoul_Text_Mon_Spectre_15:
	.string "SINISTRAIL$"
HeartSoul_Text_Mon_Spectre_16:
	.string "TREPASSABLE$"
HeartSoul_Text_Mon_Spectre_17:
	.string "SPIRITOMB$"
HeartSoul_Text_Mon_Spectre_18:
	.string "MOYADE$"
HeartSoul_Text_Mon_Spectre_19:
	.string "OSSATUEUR D'ALOLA$"

HeartSoul_EventScript_ChooseTeam_Spectre::
	call HeartSoul_EventScript_PickOne_Spectre
	copyvar VAR_0x8004, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Spectre_Slot2::
	call HeartSoul_EventScript_PickOne_Spectre
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Spectre_Slot2
	copyvar VAR_0x8005, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Spectre_Slot3::
	call HeartSoul_EventScript_PickOne_Spectre
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Spectre_Slot3
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Spectre_Slot3
	copyvar VAR_0x8006, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Spectre_Slot4::
	call HeartSoul_EventScript_PickOne_Spectre
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Spectre_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Spectre_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8006
	goto_if_eq HeartSoul_ChooseTeam_Spectre_Slot4
	copyvar VAR_0x8007, VAR_TEMP_SPECIES
	givemon VAR_0x8004, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8005, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8006, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8007, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	setflag FLAG_SYS_POKEMON_GET
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox HeartSoul_Text_ChoisisAttaquesIntro, MSGBOX_DEFAULT
HeartSoul_ChooseMoves_Spectre_ChooseMon::
	msgbox HeartSoul_Text_ChoisirPokemonAttaque, MSGBOX_DEFAULT
	setmoverelearnerstate MOVE_RELEARNER_LEVEL_UP_MOVES
	chooseboxmon SELECT_PC_MON_MOVE_RELEARNER
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartSoul_EventScript_CinnabarAttack
	special HasMovesToRelearn
	goto_if_eq VAR_RESULT, FALSE, HeartSoul_ChooseMoves_Spectre_NoMove
	msgbox HeartSoul_Text_QuelleAttaque, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto HeartSoul_ChooseMoves_Spectre_ChooseMon
HeartSoul_ChooseMoves_Spectre_NoMove:
	msgbox HeartSoul_Text_PlusRienAApprendre, MSGBOX_DEFAULT
	goto HeartSoul_ChooseMoves_Spectre_ChooseMon
	end

HeartSoul_EventScript_PickOne_Dragon::
	dynmultipush HeartSoul_Text_Mon_Dragon_0, 0
	dynmultipush HeartSoul_Text_Mon_Dragon_1, 1
	dynmultipush HeartSoul_Text_Mon_Dragon_2, 2
	dynmultipush HeartSoul_Text_Mon_Dragon_3, 3
	dynmultipush HeartSoul_Text_Mon_Dragon_4, 4
	dynmultipush HeartSoul_Text_Mon_Dragon_5, 5
	dynmultipush HeartSoul_Text_Mon_Dragon_6, 6
	dynmultipush HeartSoul_Text_Mon_Dragon_7, 7
	dynmultipush HeartSoul_Text_Mon_Dragon_8, 8
	dynmultipush HeartSoul_Text_Mon_Dragon_9, 9
	dynmultipush HeartSoul_Text_Mon_Dragon_10, 10
	dynmultipush HeartSoul_Text_Mon_Dragon_11, 11
	dynmultipush HeartSoul_Text_Mon_Dragon_12, 12
	dynmultipush HeartSoul_Text_Mon_Dragon_13, 13
	dynmultipush HeartSoul_Text_Mon_Dragon_14, 14
	dynmultipush HeartSoul_Text_Mon_Dragon_15, 15
	dynmultipush HeartSoul_Text_Mon_Dragon_16, 16
	dynmultipush HeartSoul_Text_Mon_Dragon_17, 17
	dynmultipush HeartSoul_Text_Mon_Dragon_18, 18
	dynmultipush HeartSoul_Text_Mon_Dragon_19, 19
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_PickOne_Dragon
	switch VAR_RESULT
	case 0, HeartSoul_PickOne_Dragon_Case0
	case 1, HeartSoul_PickOne_Dragon_Case1
	case 2, HeartSoul_PickOne_Dragon_Case2
	case 3, HeartSoul_PickOne_Dragon_Case3
	case 4, HeartSoul_PickOne_Dragon_Case4
	case 5, HeartSoul_PickOne_Dragon_Case5
	case 6, HeartSoul_PickOne_Dragon_Case6
	case 7, HeartSoul_PickOne_Dragon_Case7
	case 8, HeartSoul_PickOne_Dragon_Case8
	case 9, HeartSoul_PickOne_Dragon_Case9
	case 10, HeartSoul_PickOne_Dragon_Case10
	case 11, HeartSoul_PickOne_Dragon_Case11
	case 12, HeartSoul_PickOne_Dragon_Case12
	case 13, HeartSoul_PickOne_Dragon_Case13
	case 14, HeartSoul_PickOne_Dragon_Case14
	case 15, HeartSoul_PickOne_Dragon_Case15
	case 16, HeartSoul_PickOne_Dragon_Case16
	case 17, HeartSoul_PickOne_Dragon_Case17
	case 18, HeartSoul_PickOne_Dragon_Case18
	case 19, HeartSoul_PickOne_Dragon_Case19
	goto HeartSoul_EventScript_PickOne_Dragon
	end

HeartSoul_PickOne_Dragon_Case0:
	setvar VAR_TEMP_SPECIES, SPECIES_DRAGONITE
	return
HeartSoul_PickOne_Dragon_Case1:
	setvar VAR_TEMP_SPECIES, SPECIES_KINGDRA
	return
HeartSoul_PickOne_Dragon_Case2:
	setvar VAR_TEMP_SPECIES, SPECIES_SALAMENCE
	return
HeartSoul_PickOne_Dragon_Case3:
	setvar VAR_TEMP_SPECIES, SPECIES_GARCHOMP
	return
HeartSoul_PickOne_Dragon_Case4:
	setvar VAR_TEMP_SPECIES, SPECIES_FLYGON
	return
HeartSoul_PickOne_Dragon_Case5:
	setvar VAR_TEMP_SPECIES, SPECIES_ALTARIA
	return
HeartSoul_PickOne_Dragon_Case6:
	setvar VAR_TEMP_SPECIES, SPECIES_HAXORUS
	return
HeartSoul_PickOne_Dragon_Case7:
	setvar VAR_TEMP_SPECIES, SPECIES_HYDREIGON
	return
HeartSoul_PickOne_Dragon_Case8:
	setvar VAR_TEMP_SPECIES, SPECIES_GOODRA
	return
HeartSoul_PickOne_Dragon_Case9:
	setvar VAR_TEMP_SPECIES, SPECIES_NOIVERN
	return
HeartSoul_PickOne_Dragon_Case10:
	setvar VAR_TEMP_SPECIES, SPECIES_DRAGALGE
	return
HeartSoul_PickOne_Dragon_Case11:
	setvar VAR_TEMP_SPECIES, SPECIES_TURTONATOR
	return
HeartSoul_PickOne_Dragon_Case12:
	setvar VAR_TEMP_SPECIES, SPECIES_DRAMPA
	return
HeartSoul_PickOne_Dragon_Case13:
	setvar VAR_TEMP_SPECIES, SPECIES_DURALUDON
	return
HeartSoul_PickOne_Dragon_Case14:
	setvar VAR_TEMP_SPECIES, SPECIES_KOMMO_O
	return
HeartSoul_PickOne_Dragon_Case15:
	setvar VAR_TEMP_SPECIES, SPECIES_DRUDDIGON
	return
HeartSoul_PickOne_Dragon_Case16:
	setvar VAR_TEMP_SPECIES, SPECIES_TYRANTRUM
	return
HeartSoul_PickOne_Dragon_Case17:
	setvar VAR_TEMP_SPECIES, SPECIES_FLAPPLE
	return
HeartSoul_PickOne_Dragon_Case18:
	setvar VAR_TEMP_SPECIES, SPECIES_APPLETUN
	return
HeartSoul_PickOne_Dragon_Case19:
	setvar VAR_TEMP_SPECIES, SPECIES_DRAGAPULT
	return

HeartSoul_Text_Mon_Dragon_0:
	.string "DRACOLOSSE$"
HeartSoul_Text_Mon_Dragon_1:
	.string "HYPOROI$"
HeartSoul_Text_Mon_Dragon_2:
	.string "DRATTAK$"
HeartSoul_Text_Mon_Dragon_3:
	.string "CARCHACROK$"
HeartSoul_Text_Mon_Dragon_4:
	.string "LIBÉGON$"
HeartSoul_Text_Mon_Dragon_5:
	.string "ALTARIA$"
HeartSoul_Text_Mon_Dragon_6:
	.string "TRANCHODON$"
HeartSoul_Text_Mon_Dragon_7:
	.string "TRIOXHYDRE$"
HeartSoul_Text_Mon_Dragon_8:
	.string "MUPLODOCUS$"
HeartSoul_Text_Mon_Dragon_9:
	.string "BRUYVERNE$"
HeartSoul_Text_Mon_Dragon_10:
	.string "KRAVARECH$"
HeartSoul_Text_Mon_Dragon_11:
	.string "BOUMATA$"
HeartSoul_Text_Mon_Dragon_12:
	.string "DRAÏEUL$"
HeartSoul_Text_Mon_Dragon_13:
	.string "DURALUGON$"
HeartSoul_Text_Mon_Dragon_14:
	.string "ÉKAÏSER$"
HeartSoul_Text_Mon_Dragon_15:
	.string "DRAKKARMIN$"
HeartSoul_Text_Mon_Dragon_16:
	.string "REXILLIUS$"
HeartSoul_Text_Mon_Dragon_17:
	.string "POMDRAPI$"
HeartSoul_Text_Mon_Dragon_18:
	.string "DRATATIN$"
HeartSoul_Text_Mon_Dragon_19:
	.string "LANSSORIEN$"

HeartSoul_EventScript_ChooseTeam_Dragon::
	call HeartSoul_EventScript_PickOne_Dragon
	copyvar VAR_0x8004, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Dragon_Slot2::
	call HeartSoul_EventScript_PickOne_Dragon
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Dragon_Slot2
	copyvar VAR_0x8005, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Dragon_Slot3::
	call HeartSoul_EventScript_PickOne_Dragon
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Dragon_Slot3
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Dragon_Slot3
	copyvar VAR_0x8006, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Dragon_Slot4::
	call HeartSoul_EventScript_PickOne_Dragon
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Dragon_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Dragon_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8006
	goto_if_eq HeartSoul_ChooseTeam_Dragon_Slot4
	copyvar VAR_0x8007, VAR_TEMP_SPECIES
	givemon VAR_0x8004, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8005, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8006, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8007, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	setflag FLAG_SYS_POKEMON_GET
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox HeartSoul_Text_ChoisisAttaquesIntro, MSGBOX_DEFAULT
HeartSoul_ChooseMoves_Dragon_ChooseMon::
	msgbox HeartSoul_Text_ChoisirPokemonAttaque, MSGBOX_DEFAULT
	setmoverelearnerstate MOVE_RELEARNER_LEVEL_UP_MOVES
	chooseboxmon SELECT_PC_MON_MOVE_RELEARNER
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartSoul_EventScript_CinnabarAttack
	special HasMovesToRelearn
	goto_if_eq VAR_RESULT, FALSE, HeartSoul_ChooseMoves_Dragon_NoMove
	msgbox HeartSoul_Text_QuelleAttaque, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto HeartSoul_ChooseMoves_Dragon_ChooseMon
HeartSoul_ChooseMoves_Dragon_NoMove:
	msgbox HeartSoul_Text_PlusRienAApprendre, MSGBOX_DEFAULT
	goto HeartSoul_ChooseMoves_Dragon_ChooseMon
	end

HeartSoul_EventScript_PickOne_Tenebres::
	dynmultipush HeartSoul_Text_Mon_Tenebres_0, 0
	dynmultipush HeartSoul_Text_Mon_Tenebres_1, 1
	dynmultipush HeartSoul_Text_Mon_Tenebres_2, 2
	dynmultipush HeartSoul_Text_Mon_Tenebres_3, 3
	dynmultipush HeartSoul_Text_Mon_Tenebres_4, 4
	dynmultipush HeartSoul_Text_Mon_Tenebres_5, 5
	dynmultipush HeartSoul_Text_Mon_Tenebres_6, 6
	dynmultipush HeartSoul_Text_Mon_Tenebres_7, 7
	dynmultipush HeartSoul_Text_Mon_Tenebres_8, 8
	dynmultipush HeartSoul_Text_Mon_Tenebres_9, 9
	dynmultipush HeartSoul_Text_Mon_Tenebres_10, 10
	dynmultipush HeartSoul_Text_Mon_Tenebres_11, 11
	dynmultipush HeartSoul_Text_Mon_Tenebres_12, 12
	dynmultipush HeartSoul_Text_Mon_Tenebres_13, 13
	dynmultipush HeartSoul_Text_Mon_Tenebres_14, 14
	dynmultipush HeartSoul_Text_Mon_Tenebres_15, 15
	dynmultipush HeartSoul_Text_Mon_Tenebres_16, 16
	dynmultipush HeartSoul_Text_Mon_Tenebres_17, 17
	dynmultipush HeartSoul_Text_Mon_Tenebres_18, 18
	dynmultipush HeartSoul_Text_Mon_Tenebres_19, 19
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_PickOne_Tenebres
	switch VAR_RESULT
	case 0, HeartSoul_PickOne_Tenebres_Case0
	case 1, HeartSoul_PickOne_Tenebres_Case1
	case 2, HeartSoul_PickOne_Tenebres_Case2
	case 3, HeartSoul_PickOne_Tenebres_Case3
	case 4, HeartSoul_PickOne_Tenebres_Case4
	case 5, HeartSoul_PickOne_Tenebres_Case5
	case 6, HeartSoul_PickOne_Tenebres_Case6
	case 7, HeartSoul_PickOne_Tenebres_Case7
	case 8, HeartSoul_PickOne_Tenebres_Case8
	case 9, HeartSoul_PickOne_Tenebres_Case9
	case 10, HeartSoul_PickOne_Tenebres_Case10
	case 11, HeartSoul_PickOne_Tenebres_Case11
	case 12, HeartSoul_PickOne_Tenebres_Case12
	case 13, HeartSoul_PickOne_Tenebres_Case13
	case 14, HeartSoul_PickOne_Tenebres_Case14
	case 15, HeartSoul_PickOne_Tenebres_Case15
	case 16, HeartSoul_PickOne_Tenebres_Case16
	case 17, HeartSoul_PickOne_Tenebres_Case17
	case 18, HeartSoul_PickOne_Tenebres_Case18
	case 19, HeartSoul_PickOne_Tenebres_Case19
	goto HeartSoul_EventScript_PickOne_Tenebres
	end

HeartSoul_PickOne_Tenebres_Case0:
	setvar VAR_TEMP_SPECIES, SPECIES_TYRANITAR
	return
HeartSoul_PickOne_Tenebres_Case1:
	setvar VAR_TEMP_SPECIES, SPECIES_HOUNDOOM
	return
HeartSoul_PickOne_Tenebres_Case2:
	setvar VAR_TEMP_SPECIES, SPECIES_ABSOL
	return
HeartSoul_PickOne_Tenebres_Case3:
	setvar VAR_TEMP_SPECIES, SPECIES_WEAVILE
	return
HeartSoul_PickOne_Tenebres_Case4:
	setvar VAR_TEMP_SPECIES, SPECIES_HONCHKROW
	return
HeartSoul_PickOne_Tenebres_Case5:
	setvar VAR_TEMP_SPECIES, SPECIES_KROOKODILE
	return
HeartSoul_PickOne_Tenebres_Case6:
	setvar VAR_TEMP_SPECIES, SPECIES_BISHARP
	return
HeartSoul_PickOne_Tenebres_Case7:
	setvar VAR_TEMP_SPECIES, SPECIES_UMBREON
	return
HeartSoul_PickOne_Tenebres_Case8:
	setvar VAR_TEMP_SPECIES, SPECIES_SABLEYE
	return
HeartSoul_PickOne_Tenebres_Case9:
	setvar VAR_TEMP_SPECIES, SPECIES_SCRAFTY
	return
HeartSoul_PickOne_Tenebres_Case10:
	setvar VAR_TEMP_SPECIES, SPECIES_MALAMAR
	return
HeartSoul_PickOne_Tenebres_Case11:
	setvar VAR_TEMP_SPECIES, SPECIES_MANDIBUZZ
	return
HeartSoul_PickOne_Tenebres_Case12:
	setvar VAR_TEMP_SPECIES, SPECIES_ZOROARK
	return
HeartSoul_PickOne_Tenebres_Case13:
	setvar VAR_TEMP_SPECIES, SPECIES_HYDREIGON
	return
HeartSoul_PickOne_Tenebres_Case14:
	setvar VAR_TEMP_SPECIES, SPECIES_DRAPION
	return
HeartSoul_PickOne_Tenebres_Case15:
	setvar VAR_TEMP_SPECIES, SPECIES_SKUNTANK
	return
HeartSoul_PickOne_Tenebres_Case16:
	setvar VAR_TEMP_SPECIES, SPECIES_PANGORO
	return
HeartSoul_PickOne_Tenebres_Case17:
	setvar VAR_TEMP_SPECIES, SPECIES_GRIMMSNARL
	return
HeartSoul_PickOne_Tenebres_Case18:
	setvar VAR_TEMP_SPECIES, SPECIES_OBSTAGOON
	return
HeartSoul_PickOne_Tenebres_Case19:
	setvar VAR_TEMP_SPECIES, SPECIES_INCINEROAR
	return

HeartSoul_Text_Mon_Tenebres_0:
	.string "TYRANOCIF$"
HeartSoul_Text_Mon_Tenebres_1:
	.string "DÉMOLOSSE$"
HeartSoul_Text_Mon_Tenebres_2:
	.string "ABSOL$"
HeartSoul_Text_Mon_Tenebres_3:
	.string "DIMORET$"
HeartSoul_Text_Mon_Tenebres_4:
	.string "CORBOSS$"
HeartSoul_Text_Mon_Tenebres_5:
	.string "CROCORIBLE$"
HeartSoul_Text_Mon_Tenebres_6:
	.string "SCALPROIE$"
HeartSoul_Text_Mon_Tenebres_7:
	.string "NOCTALI$"
HeartSoul_Text_Mon_Tenebres_8:
	.string "TÉNÉFIX$"
HeartSoul_Text_Mon_Tenebres_9:
	.string "BAGGAÏD$"
HeartSoul_Text_Mon_Tenebres_10:
	.string "SEPIATROCE$"
HeartSoul_Text_Mon_Tenebres_11:
	.string "VAUTUTRICE$"
HeartSoul_Text_Mon_Tenebres_12:
	.string "ZOROARK$"
HeartSoul_Text_Mon_Tenebres_13:
	.string "TRIOXHYDRE$"
HeartSoul_Text_Mon_Tenebres_14:
	.string "DRASCORE$"
HeartSoul_Text_Mon_Tenebres_15:
	.string "MOUFFLAIR$"
HeartSoul_Text_Mon_Tenebres_16:
	.string "PANDARBARE$"
HeartSoul_Text_Mon_Tenebres_17:
	.string "ANGOLIATH$"
HeartSoul_Text_Mon_Tenebres_18:
	.string "IXON$"
HeartSoul_Text_Mon_Tenebres_19:
	.string "FELINFERNO$"

HeartSoul_EventScript_ChooseTeam_Tenebres::
	call HeartSoul_EventScript_PickOne_Tenebres
	copyvar VAR_0x8004, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Tenebres_Slot2::
	call HeartSoul_EventScript_PickOne_Tenebres
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Tenebres_Slot2
	copyvar VAR_0x8005, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Tenebres_Slot3::
	call HeartSoul_EventScript_PickOne_Tenebres
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Tenebres_Slot3
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Tenebres_Slot3
	copyvar VAR_0x8006, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Tenebres_Slot4::
	call HeartSoul_EventScript_PickOne_Tenebres
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Tenebres_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Tenebres_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8006
	goto_if_eq HeartSoul_ChooseTeam_Tenebres_Slot4
	copyvar VAR_0x8007, VAR_TEMP_SPECIES
	givemon VAR_0x8004, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8005, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8006, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8007, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	setflag FLAG_SYS_POKEMON_GET
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox HeartSoul_Text_ChoisisAttaquesIntro, MSGBOX_DEFAULT
HeartSoul_ChooseMoves_Tenebres_ChooseMon::
	msgbox HeartSoul_Text_ChoisirPokemonAttaque, MSGBOX_DEFAULT
	setmoverelearnerstate MOVE_RELEARNER_LEVEL_UP_MOVES
	chooseboxmon SELECT_PC_MON_MOVE_RELEARNER
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartSoul_EventScript_CinnabarAttack
	special HasMovesToRelearn
	goto_if_eq VAR_RESULT, FALSE, HeartSoul_ChooseMoves_Tenebres_NoMove
	msgbox HeartSoul_Text_QuelleAttaque, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto HeartSoul_ChooseMoves_Tenebres_ChooseMon
HeartSoul_ChooseMoves_Tenebres_NoMove:
	msgbox HeartSoul_Text_PlusRienAApprendre, MSGBOX_DEFAULT
	goto HeartSoul_ChooseMoves_Tenebres_ChooseMon
	end

HeartSoul_EventScript_PickOne_Acier::
	dynmultipush HeartSoul_Text_Mon_Acier_0, 0
	dynmultipush HeartSoul_Text_Mon_Acier_1, 1
	dynmultipush HeartSoul_Text_Mon_Acier_2, 2
	dynmultipush HeartSoul_Text_Mon_Acier_3, 3
	dynmultipush HeartSoul_Text_Mon_Acier_4, 4
	dynmultipush HeartSoul_Text_Mon_Acier_5, 5
	dynmultipush HeartSoul_Text_Mon_Acier_6, 6
	dynmultipush HeartSoul_Text_Mon_Acier_7, 7
	dynmultipush HeartSoul_Text_Mon_Acier_8, 8
	dynmultipush HeartSoul_Text_Mon_Acier_9, 9
	dynmultipush HeartSoul_Text_Mon_Acier_10, 10
	dynmultipush HeartSoul_Text_Mon_Acier_11, 11
	dynmultipush HeartSoul_Text_Mon_Acier_12, 12
	dynmultipush HeartSoul_Text_Mon_Acier_13, 13
	dynmultipush HeartSoul_Text_Mon_Acier_14, 14
	dynmultipush HeartSoul_Text_Mon_Acier_15, 15
	dynmultipush HeartSoul_Text_Mon_Acier_16, 16
	dynmultipush HeartSoul_Text_Mon_Acier_17, 17
	dynmultipush HeartSoul_Text_Mon_Acier_18, 18
	dynmultipush HeartSoul_Text_Mon_Acier_19, 19
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_PickOne_Acier
	switch VAR_RESULT
	case 0, HeartSoul_PickOne_Acier_Case0
	case 1, HeartSoul_PickOne_Acier_Case1
	case 2, HeartSoul_PickOne_Acier_Case2
	case 3, HeartSoul_PickOne_Acier_Case3
	case 4, HeartSoul_PickOne_Acier_Case4
	case 5, HeartSoul_PickOne_Acier_Case5
	case 6, HeartSoul_PickOne_Acier_Case6
	case 7, HeartSoul_PickOne_Acier_Case7
	case 8, HeartSoul_PickOne_Acier_Case8
	case 9, HeartSoul_PickOne_Acier_Case9
	case 10, HeartSoul_PickOne_Acier_Case10
	case 11, HeartSoul_PickOne_Acier_Case11
	case 12, HeartSoul_PickOne_Acier_Case12
	case 13, HeartSoul_PickOne_Acier_Case13
	case 14, HeartSoul_PickOne_Acier_Case14
	case 15, HeartSoul_PickOne_Acier_Case15
	case 16, HeartSoul_PickOne_Acier_Case16
	case 17, HeartSoul_PickOne_Acier_Case17
	case 18, HeartSoul_PickOne_Acier_Case18
	case 19, HeartSoul_PickOne_Acier_Case19
	goto HeartSoul_EventScript_PickOne_Acier
	end

HeartSoul_PickOne_Acier_Case0:
	setvar VAR_TEMP_SPECIES, SPECIES_STEELIX
	return
HeartSoul_PickOne_Acier_Case1:
	setvar VAR_TEMP_SPECIES, SPECIES_SKARMORY
	return
HeartSoul_PickOne_Acier_Case2:
	setvar VAR_TEMP_SPECIES, SPECIES_METAGROSS
	return
HeartSoul_PickOne_Acier_Case3:
	setvar VAR_TEMP_SPECIES, SPECIES_AGGRON
	return
HeartSoul_PickOne_Acier_Case4:
	setvar VAR_TEMP_SPECIES, SPECIES_EMPOLEON
	return
HeartSoul_PickOne_Acier_Case5:
	setvar VAR_TEMP_SPECIES, SPECIES_BISHARP
	return
HeartSoul_PickOne_Acier_Case6:
	setvar VAR_TEMP_SPECIES, SPECIES_SCIZOR
	return
HeartSoul_PickOne_Acier_Case7:
	setvar VAR_TEMP_SPECIES, SPECIES_EXCADRILL
	return
HeartSoul_PickOne_Acier_Case8:
	setvar VAR_TEMP_SPECIES, SPECIES_FERROTHORN
	return
HeartSoul_PickOne_Acier_Case9:
	setvar VAR_TEMP_SPECIES, SPECIES_KLINKLANG
	return
HeartSoul_PickOne_Acier_Case10:
	setvar VAR_TEMP_SPECIES, SPECIES_BRONZONG
	return
HeartSoul_PickOne_Acier_Case11:
	setvar VAR_TEMP_SPECIES, SPECIES_PROBOPASS
	return
HeartSoul_PickOne_Acier_Case12:
	setvar VAR_TEMP_SPECIES, SPECIES_CORVIKNIGHT
	return
HeartSoul_PickOne_Acier_Case13:
	setvar VAR_TEMP_SPECIES, SPECIES_AEGISLASH
	return
HeartSoul_PickOne_Acier_Case14:
	setvar VAR_TEMP_SPECIES, SPECIES_DURALUDON
	return
HeartSoul_PickOne_Acier_Case15:
	setvar VAR_TEMP_SPECIES, SPECIES_MAGNEZONE
	return
HeartSoul_PickOne_Acier_Case16:
	setvar VAR_TEMP_SPECIES, SPECIES_LUCARIO
	return
HeartSoul_PickOne_Acier_Case17:
	setvar VAR_TEMP_SPECIES, SPECIES_MAWILE
	return
HeartSoul_PickOne_Acier_Case18:
	setvar VAR_TEMP_SPECIES, SPECIES_COPPERAJAH
	return
HeartSoul_PickOne_Acier_Case19:
	setvar VAR_TEMP_SPECIES, SPECIES_FORRETRESS
	return

HeartSoul_Text_Mon_Acier_0:
	.string "STEELIX$"
HeartSoul_Text_Mon_Acier_1:
	.string "AIRMURE$"
HeartSoul_Text_Mon_Acier_2:
	.string "METALOSSE$"
HeartSoul_Text_Mon_Acier_3:
	.string "GALEKING$"
HeartSoul_Text_Mon_Acier_4:
	.string "PINGOLÉON$"
HeartSoul_Text_Mon_Acier_5:
	.string "SCALPROIE$"
HeartSoul_Text_Mon_Acier_6:
	.string "CIZAYOX$"
HeartSoul_Text_Mon_Acier_7:
	.string "MINOTAUPE$"
HeartSoul_Text_Mon_Acier_8:
	.string "NOACIER$"
HeartSoul_Text_Mon_Acier_9:
	.string "CLITICLIC$"
HeartSoul_Text_Mon_Acier_10:
	.string "ARCHEODONG$"
HeartSoul_Text_Mon_Acier_11:
	.string "TARINORME$"
HeartSoul_Text_Mon_Acier_12:
	.string "CORVAILLUS$"
HeartSoul_Text_Mon_Acier_13:
	.string "EXAGIDE$"
HeartSoul_Text_Mon_Acier_14:
	.string "DURALUGON$"
HeartSoul_Text_Mon_Acier_15:
	.string "MAGNÉZONE$"
HeartSoul_Text_Mon_Acier_16:
	.string "LUCARIO$"
HeartSoul_Text_Mon_Acier_17:
	.string "MYSDIBULE$"
HeartSoul_Text_Mon_Acier_18:
	.string "PACHYRADJAH$"
HeartSoul_Text_Mon_Acier_19:
	.string "FORETRESS$"

HeartSoul_EventScript_ChooseTeam_Acier::
	call HeartSoul_EventScript_PickOne_Acier
	copyvar VAR_0x8004, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Acier_Slot2::
	call HeartSoul_EventScript_PickOne_Acier
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Acier_Slot2
	copyvar VAR_0x8005, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Acier_Slot3::
	call HeartSoul_EventScript_PickOne_Acier
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Acier_Slot3
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Acier_Slot3
	copyvar VAR_0x8006, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Acier_Slot4::
	call HeartSoul_EventScript_PickOne_Acier
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Acier_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Acier_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8006
	goto_if_eq HeartSoul_ChooseTeam_Acier_Slot4
	copyvar VAR_0x8007, VAR_TEMP_SPECIES
	givemon VAR_0x8004, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8005, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8006, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8007, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	setflag FLAG_SYS_POKEMON_GET
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox HeartSoul_Text_ChoisisAttaquesIntro, MSGBOX_DEFAULT
HeartSoul_ChooseMoves_Acier_ChooseMon::
	msgbox HeartSoul_Text_ChoisirPokemonAttaque, MSGBOX_DEFAULT
	setmoverelearnerstate MOVE_RELEARNER_LEVEL_UP_MOVES
	chooseboxmon SELECT_PC_MON_MOVE_RELEARNER
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartSoul_EventScript_CinnabarAttack
	special HasMovesToRelearn
	goto_if_eq VAR_RESULT, FALSE, HeartSoul_ChooseMoves_Acier_NoMove
	msgbox HeartSoul_Text_QuelleAttaque, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto HeartSoul_ChooseMoves_Acier_ChooseMon
HeartSoul_ChooseMoves_Acier_NoMove:
	msgbox HeartSoul_Text_PlusRienAApprendre, MSGBOX_DEFAULT
	goto HeartSoul_ChooseMoves_Acier_ChooseMon
	end

HeartSoul_EventScript_PickOne_Fee::
	dynmultipush HeartSoul_Text_Mon_Fee_0, 0
	dynmultipush HeartSoul_Text_Mon_Fee_1, 1
	dynmultipush HeartSoul_Text_Mon_Fee_2, 2
	dynmultipush HeartSoul_Text_Mon_Fee_3, 3
	dynmultipush HeartSoul_Text_Mon_Fee_4, 4
	dynmultipush HeartSoul_Text_Mon_Fee_5, 5
	dynmultipush HeartSoul_Text_Mon_Fee_6, 6
	dynmultipush HeartSoul_Text_Mon_Fee_7, 7
	dynmultipush HeartSoul_Text_Mon_Fee_8, 8
	dynmultipush HeartSoul_Text_Mon_Fee_9, 9
	dynmultipush HeartSoul_Text_Mon_Fee_10, 10
	dynmultipush HeartSoul_Text_Mon_Fee_11, 11
	dynmultipush HeartSoul_Text_Mon_Fee_12, 12
	dynmultipush HeartSoul_Text_Mon_Fee_13, 13
	dynmultipush HeartSoul_Text_Mon_Fee_14, 14
	dynmultipush HeartSoul_Text_Mon_Fee_15, 15
	dynmultipush HeartSoul_Text_Mon_Fee_16, 16
	dynmultipush HeartSoul_Text_Mon_Fee_17, 17
	dynmultipush HeartSoul_Text_Mon_Fee_18, 18
	dynmultipush HeartSoul_Text_Mon_Fee_19, 19
	dynmultistack 0, 0, TRUE, 8, FALSE, 0, 0
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq HeartSoul_EventScript_PickOne_Fee
	switch VAR_RESULT
	case 0, HeartSoul_PickOne_Fee_Case0
	case 1, HeartSoul_PickOne_Fee_Case1
	case 2, HeartSoul_PickOne_Fee_Case2
	case 3, HeartSoul_PickOne_Fee_Case3
	case 4, HeartSoul_PickOne_Fee_Case4
	case 5, HeartSoul_PickOne_Fee_Case5
	case 6, HeartSoul_PickOne_Fee_Case6
	case 7, HeartSoul_PickOne_Fee_Case7
	case 8, HeartSoul_PickOne_Fee_Case8
	case 9, HeartSoul_PickOne_Fee_Case9
	case 10, HeartSoul_PickOne_Fee_Case10
	case 11, HeartSoul_PickOne_Fee_Case11
	case 12, HeartSoul_PickOne_Fee_Case12
	case 13, HeartSoul_PickOne_Fee_Case13
	case 14, HeartSoul_PickOne_Fee_Case14
	case 15, HeartSoul_PickOne_Fee_Case15
	case 16, HeartSoul_PickOne_Fee_Case16
	case 17, HeartSoul_PickOne_Fee_Case17
	case 18, HeartSoul_PickOne_Fee_Case18
	case 19, HeartSoul_PickOne_Fee_Case19
	goto HeartSoul_EventScript_PickOne_Fee
	end

HeartSoul_PickOne_Fee_Case0:
	setvar VAR_TEMP_SPECIES, SPECIES_CLEFABLE
	return
HeartSoul_PickOne_Fee_Case1:
	setvar VAR_TEMP_SPECIES, SPECIES_TOGEKISS
	return
HeartSoul_PickOne_Fee_Case2:
	setvar VAR_TEMP_SPECIES, SPECIES_GARDEVOIR
	return
HeartSoul_PickOne_Fee_Case3:
	setvar VAR_TEMP_SPECIES, SPECIES_SYLVEON
	return
HeartSoul_PickOne_Fee_Case4:
	setvar VAR_TEMP_SPECIES, SPECIES_GRANBULL
	return
HeartSoul_PickOne_Fee_Case5:
	setvar VAR_TEMP_SPECIES, SPECIES_MAWILE
	return
HeartSoul_PickOne_Fee_Case6:
	setvar VAR_TEMP_SPECIES, SPECIES_AZUMARILL
	return
HeartSoul_PickOne_Fee_Case7:
	setvar VAR_TEMP_SPECIES, SPECIES_WHIMSICOTT
	return
HeartSoul_PickOne_Fee_Case8:
	setvar VAR_TEMP_SPECIES, SPECIES_FLORGES
	return
HeartSoul_PickOne_Fee_Case9:
	setvar VAR_TEMP_SPECIES, SPECIES_PRIMARINA
	return
HeartSoul_PickOne_Fee_Case10:
	setvar VAR_TEMP_SPECIES, SPECIES_RIBOMBEE
	return
HeartSoul_PickOne_Fee_Case11:
	setvar VAR_TEMP_SPECIES, SPECIES_MIMIKYU
	return
HeartSoul_PickOne_Fee_Case12:
	setvar VAR_TEMP_SPECIES, SPECIES_GRIMMSNARL
	return
HeartSoul_PickOne_Fee_Case13:
	setvar VAR_TEMP_SPECIES, SPECIES_HATTERENE
	return
HeartSoul_PickOne_Fee_Case14:
	setvar VAR_TEMP_SPECIES, SPECIES_ALCREMIE
	return
HeartSoul_PickOne_Fee_Case15:
	setvar VAR_TEMP_SPECIES, SPECIES_DEDENNE
	return
HeartSoul_PickOne_Fee_Case16:
	setvar VAR_TEMP_SPECIES, SPECIES_KLEFKI
	return
HeartSoul_PickOne_Fee_Case17:
	setvar VAR_TEMP_SPECIES, SPECIES_SLURPUFF
	return
HeartSoul_PickOne_Fee_Case18:
	setvar VAR_TEMP_SPECIES, SPECIES_NINETALES_ALOLA
	return
HeartSoul_PickOne_Fee_Case19:
	setvar VAR_TEMP_SPECIES, SPECIES_WIGGLYTUFF
	return

HeartSoul_Text_Mon_Fee_0:
	.string "MELODELFE$"
HeartSoul_Text_Mon_Fee_1:
	.string "TOGEKISS$"
HeartSoul_Text_Mon_Fee_2:
	.string "GARDEVOIR$"
HeartSoul_Text_Mon_Fee_3:
	.string "NYMPHALI$"
HeartSoul_Text_Mon_Fee_4:
	.string "GRANBULL$"
HeartSoul_Text_Mon_Fee_5:
	.string "MYSDIBULE$"
HeartSoul_Text_Mon_Fee_6:
	.string "AZUMARILL$"
HeartSoul_Text_Mon_Fee_7:
	.string "FARFADUVET$"
HeartSoul_Text_Mon_Fee_8:
	.string "FLORGES$"
HeartSoul_Text_Mon_Fee_9:
	.string "ORATORIA$"
HeartSoul_Text_Mon_Fee_10:
	.string "RUBOMBELLE$"
HeartSoul_Text_Mon_Fee_11:
	.string "MIMIQUI$"
HeartSoul_Text_Mon_Fee_12:
	.string "ANGOLIATH$"
HeartSoul_Text_Mon_Fee_13:
	.string "SORCILENCE$"
HeartSoul_Text_Mon_Fee_14:
	.string "CHARMILLY$"
HeartSoul_Text_Mon_Fee_15:
	.string "DEDENNE$"
HeartSoul_Text_Mon_Fee_16:
	.string "TROUSSELIN$"
HeartSoul_Text_Mon_Fee_17:
	.string "CUPCANAILLE$"
HeartSoul_Text_Mon_Fee_18:
	.string "FEUNARD D'ALOLA$"
HeartSoul_Text_Mon_Fee_19:
	.string "GRODOUDOU$"

HeartSoul_EventScript_ChooseTeam_Fee::
	call HeartSoul_EventScript_PickOne_Fee
	copyvar VAR_0x8004, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Fee_Slot2::
	call HeartSoul_EventScript_PickOne_Fee
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Fee_Slot2
	copyvar VAR_0x8005, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Fee_Slot3::
	call HeartSoul_EventScript_PickOne_Fee
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Fee_Slot3
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Fee_Slot3
	copyvar VAR_0x8006, VAR_TEMP_SPECIES
HeartSoul_ChooseTeam_Fee_Slot4::
	call HeartSoul_EventScript_PickOne_Fee
	compare VAR_TEMP_SPECIES, VAR_0x8004
	goto_if_eq HeartSoul_ChooseTeam_Fee_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8005
	goto_if_eq HeartSoul_ChooseTeam_Fee_Slot4
	compare VAR_TEMP_SPECIES, VAR_0x8006
	goto_if_eq HeartSoul_ChooseTeam_Fee_Slot4
	copyvar VAR_0x8007, VAR_TEMP_SPECIES
	givemon VAR_0x8004, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8005, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8006, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	givemon VAR_0x8007, 34, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, SHINY_MODE_NEVER
	setflag FLAG_SYS_POKEMON_GET
	giveitem ITEM_POKE_BALL, 5
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH
	msgbox HeartSoul_Text_ChoisisAttaquesIntro, MSGBOX_DEFAULT
HeartSoul_ChooseMoves_Fee_ChooseMon::
	msgbox HeartSoul_Text_ChoisirPokemonAttaque, MSGBOX_DEFAULT
	setmoverelearnerstate MOVE_RELEARNER_LEVEL_UP_MOVES
	chooseboxmon SELECT_PC_MON_MOVE_RELEARNER
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, HeartSoul_EventScript_CinnabarAttack
	special HasMovesToRelearn
	goto_if_eq VAR_RESULT, FALSE, HeartSoul_ChooseMoves_Fee_NoMove
	msgbox HeartSoul_Text_QuelleAttaque, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto HeartSoul_ChooseMoves_Fee_ChooseMon
HeartSoul_ChooseMoves_Fee_NoMove:
	msgbox HeartSoul_Text_PlusRienAApprendre, MSGBOX_DEFAULT
	goto HeartSoul_ChooseMoves_Fee_ChooseMon
	end

HeartSoul_Text_ChoisisAttaquesIntro:
	.string "Avant de partir, choisis les\n"
	.string "attaques de tes 4 POKEMON.\p"
	.string "Pour chacun, tu pourras remplacer\n"
	.string "une attaque par une autre qu'il\l"
	.string "peut apprendre par niveau.\p"
	.string "Quitte le menu quand tu as fini.$"

HeartSoul_Text_ChoisirPokemonAttaque:
	.string "Choisis le POKEMON dont tu veux\n"
	.string "changer une attaque.$"

HeartSoul_Text_QuelleAttaque:
	.string "Quelle attaque doit-il apprendre ?$"

HeartSoul_Text_PlusRienAApprendre:
	.string "Ce POKEMON ne peut apprendre\n"
	.string "aucune autre attaque par niveau\l"
	.string "pour l'instant.$"
