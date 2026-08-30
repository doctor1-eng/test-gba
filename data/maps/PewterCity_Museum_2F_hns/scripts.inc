PewterCity_Museum_2F_hns_MapScripts::
	.byte 0


PewterCity_Museum_2F_EventScript_Nerd::
	msgbox PewterCity_Museum_2F_Text_Nerd, MSGBOX_NPC
	end

PewterCity_Museum_2F_EventScript_Daughter::
	lock
	msgbox PewterCity_Museum_2F_Text_AskedDaddyToCatchPikachu, MSGBOX_DEFAULT
	release
	end

PewterCity_Museum_2F_EventScript_Dad::
	lock
	msgbox PewterCity_Museum_2F_Text_PikachuSoonIPromise, MSGBOX_DEFAULT
	release
	end

PewterCity_Museum_2F_EventScript_Meteor::
	msgbox PewterCity_Museum_2F_Text_MeteoriteThatFellOnMtMoon, MSGBOX_SIGN
	end

PewterCity_Museum_2F_Text_Nerd:
	.string "On dit que des météorites sont\n"
	.string "tombées sur BIRTH ISLAND.\p"
	.string "On adorerait les exposer ici\n"
	.string "dans notre musée, mais...\p"
	.string "On dit que ces météorites\n"
	.string "émettent une énergie étrange\l"
	.string "et qu'on ne peut pas les\l"
	.string "emporter...$"

PewterCity_Museum_2F_Text_AskedDaddyToCatchPikachu::
    .string "Je veux un PIKACHU !\n"
    .string "Il est trop mignon !\p"
    .string "J'ai demandé à papa de m'en\n"
    .string "attraper un !$"

PewterCity_Museum_2F_Text_PikachuSoonIPromise::
    .string "Oui, un PIKACHU bientôt, promis !$"

PewterCity_Museum_2F_Text_SpaceShuttle::
    .string "Navette spatiale$"

PewterCity_Museum_2F_Text_MeteoriteThatFellOnMtMoon::
    .string "Une météorite tombée sur MT. MOON.\n"
    .string "On pense que c'est une MOON STONE.$"
