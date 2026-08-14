MtSelenite_B2F_MapScripts::
	.byte 0

MtSelenite_B2F_EventScript_Fossil1::
	lock
	faceplayer
	msgbox MtSelenite_B2F_Text_YouWantDomeFossil, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MtSelenite_B2F_EventScript_DontTakeFossil
	giveitem_msg MtSelenite_B2F_Text_ObtainedDomeFossil, ITEM_DOME_FOSSIL, 1, MUS_RG_OBTAIN_KEY_ITEM
	setflag FLAG_MT_SELENITE_FOSSIL_TAKEN
	release
	end

MtSelenite_B2F_EventScript_Fossil2::
	lock
	faceplayer
	msgbox MtSelenite_B2F_Text_YouWantHelixFossil, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MtSelenite_B2F_EventScript_DontTakeFossil
	giveitem_msg MtSelenite_B2F_Text_ObtainedHelixFossil, ITEM_HELIX_FOSSIL, 1, MUS_RG_OBTAIN_KEY_ITEM
	setflag FLAG_MT_SELENITE_FOSSIL_TAKEN
	release
	end

MtSelenite_B2F_EventScript_DontTakeFossil::
	msgbox MtSelenite_B2F_Text_DontTakeFossil, MSGBOX_DEFAULT
	release
	end

MtSelenite_B2F_EventScript_RarePokemon::
	lock
	faceplayer
	setwildbattle SPECIES_CLEFAIRY, 12
	waitse
	playmoncry SPECIES_CLEFAIRY, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	dowildbattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtSelenite_B2F_EventScript_RarePokemonCaughtOrDefeated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MtSelenite_B2F_EventScript_RarePokemonCaughtOrDefeated
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtSelenite_B2F_EventScript_RarePokemonCaughtOrDefeated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtSelenite_B2F_EventScript_RarePokemonCaughtOrDefeated
	release
	end

MtSelenite_B2F_EventScript_RarePokemonCaughtOrDefeated::
	setflag FLAG_MT_SELENITE_RARE_MON_TAKEN
	goto Common_EventScript_RemoveStaticPokemon

MtSelenite_B2F_Text_YouWantDomeFossil:
	.string "Un fossile en forme de dôme est\n"
	.string "à moitié enfoui dans la roche.\p"
	.string "Le prendre ?$"

MtSelenite_B2F_Text_ObtainedDomeFossil:
	.string "{PLAYER} a obtenu le FOSSILE DÔME !$"

MtSelenite_B2F_Text_YouWantHelixFossil:
	.string "Un fossile en forme de spirale est\n"
	.string "à moitié enfoui dans la roche.\p"
	.string "Le prendre ?$"

MtSelenite_B2F_Text_ObtainedHelixFossil:
	.string "{PLAYER} a obtenu le FOSSILE SPIRALE !$"

MtSelenite_B2F_Text_DontTakeFossil:
	.string "{PLAYER} laisse le fossile où il est.$"
