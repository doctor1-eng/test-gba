Route102_MapScripts::
	.byte 0

Route102_EventScript_LittleBoy::
	msgbox Route102_Text_ImNotVeryTall, MSGBOX_NPC
	end

Route102_EventScript_RouteSignOldale::
	msgbox Route102_Text_RouteSignOldale, MSGBOX_SIGN
	end

Route102_EventScript_RouteSignPetalburg::
	msgbox Route102_Text_RouteSignPetalburg, MSGBOX_SIGN
	end

@ Kanto Saison 1 : première rencontre avec Ondine (Arc 3), rivale récurrente qu'on recroisera plus tard
@ dans le jeu. Objet réutilisé depuis le PNJ "Boy" d'origine (position déjà validée), pas de nouvelle
@ coordonnée inventée.
Route102_EventScript_Ondine::
	lock
	faceplayer
	msgbox Route102_Text_OndineInterruption, MSGBOX_DEFAULT
	msgbox Route102_Text_OndineChallenge, MSGBOX_DEFAULT
	closemessage
	trainerbattle_single TRAINER_ONDINE_1, Route102_Text_OndineBattleIntro, Route102_Text_OndineDefeat, Route102_EventScript_OndineAfterBattle
	end

Route102_EventScript_OndineAfterBattle::
	msgbox Route102_Text_OndineJoinParty, MSGBOX_DEFAULT
	setflag FLAG_ONDINE_ROUTE2_DEFEATED
	setflag FLAG_HIDE_ROUTE_2_ONDINE_ENCOUNTER
	release
	end

Route102_EventScript_Calvin::
	trainerbattle_single TRAINER_CALVIN_1, Route102_Text_CalvinIntro, Route102_Text_CalvinDefeated, Route102_EventScript_CalvinRegisterMatchCallAfterBattle
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route102_EventScript_CalvinRematch
	setvar VAR_0x8004, TRAINER_CALVIN_1
	specialvar VAR_RESULT, IsTrainerRegistered
	goto_if_eq VAR_RESULT, FALSE, Route102_EventScript_CalvinTryRegister
	msgbox Route102_Text_CalvinPostBattle, MSGBOX_DEFAULT
	release
	end

Route102_EventScript_CalvinRegisterMatchCallAfterBattle::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	goto_if_set FLAG_HAS_MATCH_CALL, Route102_EventScript_CalvinRegisterMatchCall
	release
	end

Route102_EventScript_CalvinRegisterMatchCall::
	msgbox Route102_Text_CalvinRegisterShort, MSGBOX_DEFAULT
	register_matchcall TRAINER_CALVIN_1
	release
	end

Route102_EventScript_CalvinTryRegister::
	goto_if_set FLAG_HAS_MATCH_CALL, Route102_EventScript_CalvinRegister
	msgbox Route102_Text_CalvinPostBattle, MSGBOX_DEFAULT
	release
	end

Route102_EventScript_CalvinRegister::
	msgbox Route102_Text_CalvinRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_CALVIN_1
	release
	end

Route102_EventScript_CalvinRematch::
	trainerbattle_rematch TRAINER_CALVIN_1, Route102_Text_CalvinRematchIntro, Route102_Text_CalvinRematchDefeated
	msgbox Route102_Text_CalvinRematchPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_Rick::
	trainerbattle_single TRAINER_RICK, Route102_Text_RickIntro, Route102_Text_RickDefeated
	msgbox Route102_Text_RickPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_Tiana::
	trainerbattle_single TRAINER_TIANA, Route102_Text_TianaIntro, Route102_Text_TianaDefeated
	msgbox Route102_Text_TianaPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_Allen::
	trainerbattle_single TRAINER_ALLEN, Route102_Text_AllenIntro, Route102_Text_AllenDefeated
	msgbox Route102_Text_AllenPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_Text_WatchMeCatchPokemon:
	.string "WALLY: {PLAYER}…\n"
	.string "POKéMON hide in tall grass like this,\l"
	.string "don't they?\p"
	.string "Please watch me and see if I can\n"
	.string "catch one properly.\p"
	.string "…Whoa!$"

Route102_Text_WallyIDidIt:
	.string "WALLY: I did it… It's my…\n"
	.string "My POKéMON!$"

Route102_Text_LetsGoBack:
	.string "{PLAYER}, thank you!\n"
	.string "Let's go back to the GYM!$"

Route102_Text_ImNotVeryTall:
	.string "Je… je ne suis pas très grand, alors\n"
	.string "je disparais dans les hautes herbes.\p"
	.string "L'herbe me chatouille le nez et…\n"
	.string "Fwafwafwafwafwa…\p"
	.string "Fwatchoo !$"

Route102_Text_OndineInterruption:
	.string "Hé ! Tu comptes laisser ce POKéMON\n"
	.string "flanquer la pagaille encore\l"
	.string "longtemps ?\p"
	.string "Je pêchais tranquillement,\n"
	.string "figure-toi !$"

Route102_Text_OndineChallenge:
	.string "Un dresseur qui ne sait même pas\n"
	.string "tenir son POKéMON en laisse…\p"
	.string "Tu ferais mieux de me prouver que\n"
	.string "tu sais au moins te battre.$"

Route102_Text_OndineBattleIntro:
	.string "Je m'appelle Ondine.\p"
	.string "Et question POKéMON EAU, personne\n"
	.string "ne m'arrive à la cheville !$"

Route102_Text_OndineDefeat:
	.string "Hmpf. Pas mal, pour un débutant.$"

Route102_Text_OndineJoinParty:
	.string "Je vais à Argenta de toute façon.\p"
	.string "Alors ne traîne pas, je te garde\n"
	.string "à l'œil.$"

Route102_Text_RouteSignOldale:
	.string "ROUTE 2\n"
	.string "{RIGHT_ARROW} JADIELLE$"

Route102_Text_RouteSignPetalburg:
	.string "ROUTE 2\n"
	.string "{LEFT_ARROW} ARGENTA$"
