VermilionCity_FanClub_hns_MapScripts::
	.byte 0

VermilionCity_FanClub_EventScript_Chairman::
	lock
	faceplayer
	goto_if_eq VAR_FAN_CLUB_GUY, 1, VermilionCity_FanClub_EventScript_ChairmanAfter
	msgbox PokemonFanClub_Text_Chairman_Intro, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, FALSE, VermilionCity_FanClub_EventScript_ChairmanRefused
	msgbox PokemonFanClub_Text_Chairman_Rapidash, MSGBOX_DEFAULT
	msgbox PokemonFanClub_Text_Chairman_Gift, MSGBOX_DEFAULT
	giveitem ITEM_RARE_CANDY
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setvar VAR_FAN_CLUB_GUY, 1
	msgbox PokemonFanClub_Text_Chairman_Candy, MSGBOX_DEFAULT
	closemessage
	release
	end

VermilionCity_FanClub_EventScript_ChairmanRefused::
	msgbox PokemonFanClub_Text_Chairman_Disappointed, MSGBOX_DEFAULT
	closemessage
	release
	end

VermilionCity_FanClub_EventScript_ChairmanAfter::
	msgbox PokemonFanClub_Text_Chairman_MoreTales, MSGBOX_DEFAULT
	closemessage
	release
	end

VermilionCity_FanClub_EventScript_Woman1::
	msgbox PokemonFanClub_Text_Receptionist, MSGBOX_NPC
	end

VermilionCity_FanClub_EventScript_Woman2::
	msgbox PokemonFanClub_Text_Teacher, MSGBOX_NPC
	end

VermilionCity_FanClub_EventScript_ClefairyGuy::
	lock
	faceplayer
	msgbox PokemonFanClub_Text_ClefairyGuy_Adorable, MSGBOX_DEFAULT
	goto_if_eq VAR_FAN_CLUB_CLEFAIRY, 1, VermilionCity_FanClub_EventScript_ClefairyGiveDoll
	goto_if_ge VAR_FAN_CLUB_CLEFAIRY, 2, VermilionCity_FanClub_EventScript_ClefairyAfter
	msgbox PokemonFanClub_Text_ClefairyGuy_MakingDo, MSGBOX_DEFAULT
	closemessage
	release
	end

VermilionCity_FanClub_EventScript_ClefairyGiveDoll::
	msgbox PokemonFanClub_Text_ClefairyGuy_ReturnDoll, MSGBOX_DEFAULT
	fadescreenswapbuffers FADE_TO_BLACK
	setflag FLAG_HIDE_FAN_CLUB_CLEFAIRY_DOLL
	removeobject LOCALID_FAN_CLUB_CLEFAIRY_DOLL
	fadescreenswapbuffers FADE_FROM_BLACK
	giveitem ITEM_LOST_ITEM
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setvar VAR_FAN_CLUB_CLEFAIRY, 2
	msgbox PokemonFanClub_Text_ClefairyGuy_GoingToGetOne, MSGBOX_DEFAULT
	closemessage
	release
	end

VermilionCity_FanClub_EventScript_ClefairyAfter::
	msgbox PokemonFanClub_Text_ClefairyGuy_GoingToGetOne, MSGBOX_DEFAULT
	closemessage
	release
	end

VermilionCity_FanClub_EventScript_Bayleef::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_BAYLEEF, CRY_MODE_NORMAL
	msgbox PokemonFanClub_Text_Bayleef, MSGBOX_DEFAULT
	waitmoncry
	release
	end


PokemonFanClub_Text_Chairman_Intro:
	.string "Je suis le PRÉSIDENT du\n"
	.string "POKéMON FAN CLUB.\p"
	.string "J'ai élevé plus de 150 POKéMON.\p"
	.string "Je suis très pointilleux quand\n"
	.string "il s'agit de POKéMON.\p"
	.string "Es-tu venu juste pour\n"
	.string "m'entendre parler de mes\l"
	.string "POKéMON?$"

PokemonFanClub_Text_Chairman_Rapidash:
	.string "Bien! Alors écoute bien!\p"
	.string "Donc… mon RAPIDASH préféré…\p"
	.string "Il est… mignon… adorable…\n"
	.string "intelligent… incroyablement…\l"
	.string "en plus… stupéfiant… oh… tu\l"
	.string "trouves aussi?…\l"
	.string "Beaucoup trop… sauvage…\l"
	.string "magnifique… si gentil…\l"
	.string "je l'adore!\p"
	.string "Le câliner… quand… il dort…\n"
	.string "tout chaud et douillet…\l"
	.string "Oh, et… spectaculaire…\l"
	.string "ravissant… tout simplement\l"
	.string "divin…\p"
	.string "Oups! Regarde l'heure!\n"
	.string "Je t'ai retenu trop longtemps!$"

PokemonFanClub_Text_Chairman_Gift:
	.string "Merci de m'avoir écouté.\n"
	.string "Je veux que tu aies ceci!$"

PokemonFanClub_Text_Chairman_Candy:
	.string "C'est un RARE CANDY qui rend\n"
	.string "les POKéMON plus forts.\p"
	.string "Je préfère renforcer mes\n"
	.string "POKéMON en les faisant\l"
	.string "combattre, alors tu peux le\l"
	.string "garder.$"

PokemonFanClub_Text_Chairman_MoreTales:
	.string "Bonjour, {PLAYER}!\p"
	.string "Es-tu venu me voir encore au\n"
	.string "sujet de mes POKéMON?\p"
	.string "Non? Oh… j'avais encore plein\n"
	.string "d'histoires à raconter…$"

PokemonFanClub_Text_Chairman_Disappointed:
	.string "Quelle déception…\p"
	.string "Reviens si tu veux bien\n"
	.string "m'écouter.$"

PokemonFanClub_Text_Receptionist:
	.string "Notre PRÉSIDENT est très\n"
	.string "bavard quand il s'agit de\l"
	.string "POKéMON…$"

@=Clefairy=Guy=========================================================================================================
PokemonFanClub_Text_ClefairyGuy_Adorable:
	.string "J'adore la façon dont CLEFAIRY\n"
	.string "agite son doigt quand il\l"
	.string "essaie d'utiliser METRONOME.\p"
	.string "C'est trop mignon!$"

PokemonFanClub_Text_ClefairyGuy_MakingDo:
	.string "J'adore CLEFAIRY, mais je n'ai\n"
	.string "jamais réussi à en attraper un.\p"
	.string "Alors je me contente d'une\n"
	.string "POKé DOLL que j'ai trouvée.$"

PokemonFanClub_Text_ClefairyGuy_ReturnDoll:
	.string "Ah, je comprends maintenant.\n"
	.string "La fille qui a perdu cette\l"
	.string "POKé DOLL est triste…\p"
	.string "D'accord. Pourrais-tu\n"
	.string "rapporter cette POKé DOLL à\l"
	.string "cette pauvre petite fille?\p"
	.string "Je me lierai d'amitié avec un\n"
	.string "vrai CLEFAIRY moi-même un\l"
	.string "jour. Ne t'en fais pas!$"

PokemonFanClub_Text_ClefairyGuy_GoingToGetOne:
	.string "Tu vas voir. Je vais me faire\n"
	.string "un vrai CLEFAIRY comme ami.$"

@============================================================================================================================

PokemonFanClub_Text_Teacher:
	.string "Regarde mon adorable BAYLEEF!\p"
	.string "La feuille sur sa tête est\n"
	.string "trop mignonne!$"

PokemonFanClub_Text_ClefairyDoll:
	.string "C'est un CLEFAIRY! Hein?\p"
	.string "Ah, mais non. C'est une POKé\n"
	.string "DOLL CLEFAIRY.$"

PokemonFanClub_Text_Bayleef:
	.string "BAYLEEF: Li liif!$"

PokemonFanClub_Text_Listen:
	.string "Écoutons tous poliment les\n"
	.string "autres dresseurs.$"

PokemonFanClub_Text_Bragging:
	.string "Si quelqu'un se vante,\n"
	.string "vante-toi en retour!$"
