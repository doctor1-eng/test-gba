// Kanto Saison 1 (suite 32) : voir graphics_kanto.h pour le contexte general.
// gMetatiles_* pointent directement sur metatiles.bin (identiques aux fichiers FRLG d'origine,
// aucune conversion necessaire : l'encodage tileId/xflip/yflip/palette est le meme des deux cotes).
// gMetatileAttributes_* pointent en revanche sur des fichiers *_emerald_behaviors.bin generes par
// tools/kanto_tileset_port/convert_frlg_behaviors.py : memes 4 octets/metatile que les binaires
// FRLG d'origine (isFrlg=TRUE conserve, voir mapjson.cpp "metatile_format": "frlg"), seule la
// VALEUR du champ Behavior est reencodee de la numerotation MB_FRLG_* vers la numerotation MB_*
// unifiee utilisee par ce moteur - necessaire, ce n'est PAS un simple copier-coller (voir
// docs/TECHNICAL_ARCHITECTURE.md pour le detail complet et la justification de chaque cas
// particulier rencontre sur ce premier test).

const u16 gMetatiles_GeneralFrlgKanto[] = INCBIN_U16("data/tilesets/primary/general_frlg/metatiles.bin");
const u16 gMetatileAttributes_GeneralFrlgKanto[] = INCBIN_U16("data/tilesets/primary/general_frlg/metatile_attributes_emerald_behaviors.bin");

const u16 gMetatiles_PalletTownKanto[] = INCBIN_U16("data/tilesets/secondary/pallet_town_frlg/metatiles.bin");
const u16 gMetatileAttributes_PalletTownKanto[] = INCBIN_U16("data/tilesets/secondary/pallet_town_frlg/metatile_attributes_emerald_behaviors.bin");

const u16 gMetatiles_ViridianCityKanto[] = INCBIN_U16("data/tilesets/secondary/viridian_city_frlg/metatiles.bin");
const u16 gMetatileAttributes_ViridianCityKanto[] = INCBIN_U16("data/tilesets/secondary/viridian_city_frlg/metatile_attributes_emerald_behaviors.bin");

const u16 gMetatiles_PewterCityKanto[] = INCBIN_U16("data/tilesets/secondary/pewter_city_frlg/metatiles.bin");
const u16 gMetatileAttributes_PewterCityKanto[] = INCBIN_U16("data/tilesets/secondary/pewter_city_frlg/metatile_attributes_emerald_behaviors.bin");
