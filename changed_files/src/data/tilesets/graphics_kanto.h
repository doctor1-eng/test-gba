// Kanto Saison 1 (suite 32) : declarations dupliquees hors du garde `#if IS_FRLG` de graphics.h,
// pour compiler les vraies tuiles/palettes Kanto FRLG dans ce build Emerald (IS_FRLG vaut 0 ici).
// Pointent vers les MEMES fichiers source que gTilesetTiles_General_Frlg/gTilesetPalettes_PalletTown
// etc. (aucune copie de donnees graphiques, juste de nouveaux symboles C compiles sans condition).
// Voir docs/TECHNICAL_ARCHITECTURE.md, section "Tilesets Kanto FRLG dans un hack Emerald".
//
// Premier test (Bourg Palette / pallet_town_frlg) valide en suite 32-34. viridian_city_frlg ajoute
// en suite 35 comme preview de la richesse d'une vraie grande ville avant de choisir laquelle des
// 13 villes restantes traiter en premier (voir docs/TECHNICAL_ARCHITECTURE.md).

const u32 gTilesetTiles_GeneralFrlgKanto[] = INCGFX_U32("data/tilesets/primary/general_frlg/tiles.png", ".4bpp.smol");

const u16 ALIGNED(4) gTilesetPalettes_GeneralFrlgKanto[][16] =
{
    INCGFX_U16("data/tilesets/primary/general_frlg/palettes/00.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/primary/general_frlg/palettes/01.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/primary/general_frlg/palettes/02.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/primary/general_frlg/palettes/03.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/primary/general_frlg/palettes/04.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/primary/general_frlg/palettes/05.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/primary/general_frlg/palettes/06.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/primary/general_frlg/palettes/07.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/primary/general_frlg/palettes/08.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/primary/general_frlg/palettes/09.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/primary/general_frlg/palettes/10.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/primary/general_frlg/palettes/11.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/primary/general_frlg/palettes/12.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/primary/general_frlg/palettes/13.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/primary/general_frlg/palettes/14.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/primary/general_frlg/palettes/15.pal", ".gbapal"),
};

const u32 gTilesetTiles_PalletTownKanto[] = INCGFX_U32("data/tilesets/secondary/pallet_town_frlg/tiles.png", ".4bpp.fastSmol");

const u16 ALIGNED(4) gTilesetPalettes_PalletTownKanto[][16] =
{
    INCGFX_U16("data/tilesets/secondary/pallet_town_frlg/palettes/00.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/pallet_town_frlg/palettes/01.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/pallet_town_frlg/palettes/02.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/pallet_town_frlg/palettes/03.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/pallet_town_frlg/palettes/04.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/pallet_town_frlg/palettes/05.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/pallet_town_frlg/palettes/06.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/pallet_town_frlg/palettes/07.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/pallet_town_frlg/palettes/08.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/pallet_town_frlg/palettes/09.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/pallet_town_frlg/palettes/10.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/pallet_town_frlg/palettes/11.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/pallet_town_frlg/palettes/12.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/pallet_town_frlg/palettes/13.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/pallet_town_frlg/palettes/14.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/pallet_town_frlg/palettes/15.pal", ".gbapal"),
};

const u32 gTilesetTiles_ViridianCityKanto[] = INCGFX_U32("data/tilesets/secondary/viridian_city_frlg/tiles.png", ".4bpp.fastSmol");

const u16 ALIGNED(4) gTilesetPalettes_ViridianCityKanto[][16] =
{
    INCGFX_U16("data/tilesets/secondary/viridian_city_frlg/palettes/00.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/viridian_city_frlg/palettes/01.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/viridian_city_frlg/palettes/02.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/viridian_city_frlg/palettes/03.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/viridian_city_frlg/palettes/04.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/viridian_city_frlg/palettes/05.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/viridian_city_frlg/palettes/06.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/viridian_city_frlg/palettes/07.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/viridian_city_frlg/palettes/08.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/viridian_city_frlg/palettes/09.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/viridian_city_frlg/palettes/10.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/viridian_city_frlg/palettes/11.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/viridian_city_frlg/palettes/12.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/viridian_city_frlg/palettes/13.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/viridian_city_frlg/palettes/14.pal", ".gbapal"),
    INCGFX_U16("data/tilesets/secondary/viridian_city_frlg/palettes/15.pal", ".gbapal"),
};
