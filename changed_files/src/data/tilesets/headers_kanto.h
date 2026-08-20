// Kanto Saison 1 (suite 32) : voir graphics_kanto.h pour le contexte general.
// Structs Tileset compilees sans condition (contrairement a gTileset_General_Frlg/gTileset_PalletTown
// qui vivent dans la branche #else (IS_FRLG uniquement) de headers.h et ne sont donc jamais compilees
// dans ce build Emerald). InitTilesetAnim_General_Frlg est deja compilee sans condition ailleurs
// (src/tileset_anims.c) et peut donc etre reutilisee telle quelle pour l'animation d'eau/fleurs.

const struct Tileset gTileset_GeneralFrlgKanto =
{
    .isCompressed = TRUE,
    .isSecondary = FALSE,
    .tiles = gTilesetTiles_GeneralFrlgKanto,
    .palettes = gTilesetPalettes_GeneralFrlgKanto,
    .metatiles = gMetatiles_GeneralFrlgKanto,
    .metatileAttributes = gMetatileAttributes_GeneralFrlgKanto,
    .callback = InitTilesetAnim_General_Frlg,
};

const struct Tileset gTileset_PalletTownKanto =
{
    .isCompressed = TRUE,
    .isSecondary = TRUE,
    .tiles = gTilesetTiles_PalletTownKanto,
    .palettes = gTilesetPalettes_PalletTownKanto,
    .metatiles = gMetatiles_PalletTownKanto,
    .metatileAttributes = gMetatileAttributes_PalletTownKanto,
    .callback = NULL,
};

const struct Tileset gTileset_ViridianCityKanto =
{
    .isCompressed = TRUE,
    .isSecondary = TRUE,
    .tiles = gTilesetTiles_ViridianCityKanto,
    .palettes = gTilesetPalettes_ViridianCityKanto,
    .metatiles = gMetatiles_ViridianCityKanto,
    .metatileAttributes = gMetatileAttributes_ViridianCityKanto,
    .callback = NULL,
};
