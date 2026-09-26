@ Heart & Soul - Arenes Libres de Celadopole (data/scripts/heart_and_soul_oublies.inc).
CeladonCity_AreneLibre_Hns_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CeladonCity_AreneLibre_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CeladonCity_AreneLibre_Hns_OnFrame
	.byte 0

CeladonCity_AreneLibre_Hns_OnFrame:
	map_script_2 VAR_TEMP_0, 0, CeladonCity_AreneLibre_EventScript_Entree
	.2byte 0
