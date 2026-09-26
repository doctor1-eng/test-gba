// Heart & Soul - opening cinematic "La chute de Cinnabar".
//
// Five full-screen pixel-art panels (tools/hns_intro/paint_panels.py -> graphics/hns_intro/)
// shown one after another with a slow pan, a letterboxed caption, fades, and a lightning
// flash + screen shake on the attack panel. Called from the Act I attack script:
//     special HnsPlayIntroCinematic
//     waitstate
// It takes over the screen with its own main callback and returns to the overworld through
// CB2_ReturnToFieldContinueScriptPlayMapMusic, which resumes the waiting script. A or B skips
// to the next panel, START skips the whole sequence.
//
// VRAM layout: BG3 = picture, 8bpp tiles in char blocks 0-2 (<= 640 tiles), map in screen
// block 30. BG0 = caption window, 4bpp, char block 3, map in screen block 31. The picture
// palette uses BG palette entries 0..223; the caption uses 4bpp palette slot 15 (240..255).
#include "global.h"
#include "bg.h"
#include "gpu_regs.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "text.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"

struct HnsPanel
{
    const u8 *tiles;
    u32 tilesSize;
    const u16 *map;
    const u16 *pal;
    u32 palSize;
    const u8 *caption;
    s8 panSpeed; // BG x step every 8 frames: positive pans right, 0 = still
    bool8 lightning;
};

static const u8 sPanel1Tiles[] = INCBIN_U8("graphics/hns_intro/panel1_tiles.bin");
static const u16 sPanel1Map[] = INCBIN_U16("graphics/hns_intro/panel1_map.bin");
static const u16 sPanel1Pal[] = INCBIN_U16("graphics/hns_intro/panel1_pal.bin");
static const u8 sPanel2Tiles[] = INCBIN_U8("graphics/hns_intro/panel2_tiles.bin");
static const u16 sPanel2Map[] = INCBIN_U16("graphics/hns_intro/panel2_map.bin");
static const u16 sPanel2Pal[] = INCBIN_U16("graphics/hns_intro/panel2_pal.bin");
static const u8 sPanel3Tiles[] = INCBIN_U8("graphics/hns_intro/panel3_tiles.bin");
static const u16 sPanel3Map[] = INCBIN_U16("graphics/hns_intro/panel3_map.bin");
static const u16 sPanel3Pal[] = INCBIN_U16("graphics/hns_intro/panel3_pal.bin");
static const u8 sPanel4Tiles[] = INCBIN_U8("graphics/hns_intro/panel4_tiles.bin");
static const u16 sPanel4Map[] = INCBIN_U16("graphics/hns_intro/panel4_map.bin");
static const u16 sPanel4Pal[] = INCBIN_U16("graphics/hns_intro/panel4_pal.bin");
static const u8 sPanel5Tiles[] = INCBIN_U8("graphics/hns_intro/panel5_tiles.bin");
static const u16 sPanel5Map[] = INCBIN_U16("graphics/hns_intro/panel5_map.bin");
static const u16 sPanel5Pal[] = INCBIN_U16("graphics/hns_intro/panel5_pal.bin");

static const u8 sCaption1[] = _("Île de CINNABAR. Minuit.\nLa ville dort au pied du volcan.");
static const u8 sCaption2[] = _("Au large, des navires sans pavillon.\nPuis, un à un, des R rouges s'allument.");
static const u8 sCaption3[] = _("BLUE: “La LIGUE n'a jamais protégé\nque son image. Ce soir, on le prouve.”");
static const u8 sCaption4[] = _("En quelques minutes, le port tombe.\nLe CENTRE POKéMON est encerclé.");
static const u8 sCaption5[] = _("HEART & SOUL\nActe I - La chute de CINNABAR");

#define PANEL(n, cap, pan, bolt) {sPanel##n##Tiles, sizeof(sPanel##n##Tiles), sPanel##n##Map, \
                                  sPanel##n##Pal, sizeof(sPanel##n##Pal), cap, pan, bolt}
static const struct HnsPanel sPanels[] =
{
    PANEL(1, sCaption1, 1, FALSE),
    PANEL(2, sCaption2, -1, FALSE),
    PANEL(3, sCaption3, 1, FALSE),
    PANEL(4, sCaption4, 0, TRUE),
    PANEL(5, sCaption5, 0, FALSE),
};

static const struct BgTemplate sBgTemplates[] =
{
    {.bg = 0, .charBaseIndex = 3, .mapBaseIndex = 31, .screenSize = 0, .paletteMode = 0, .priority = 0, .baseTile = 0},
    {.bg = 3, .charBaseIndex = 0, .mapBaseIndex = 30, .screenSize = 0, .paletteMode = 1, .priority = 3, .baseTile = 0},
};

// Letterbox: a full-width black caption band at the bottom and a thin band at the top.
#define WIN_CAPTION 0
#define WIN_TOPBAR  1
static const struct WindowTemplate sWindowTemplates[] =
{
    [WIN_CAPTION] = {.bg = 0, .tilemapLeft = 0, .tilemapTop = 16, .width = 30, .height = 4, .paletteNum = 15, .baseBlock = 1},
    [WIN_TOPBAR]  = {.bg = 0, .tilemapLeft = 0, .tilemapTop = 0, .width = 30, .height = 1, .paletteNum = 15, .baseBlock = 1 + 30 * 4},
    DUMMY_WIN_TEMPLATE,
};

// caption palette (slot 15): 0 transparent, 1 black strip, 2 white text, 3 grey shadow
static const u16 sCaptionPal[16] = {RGB_BLACK, RGB(1, 1, 3), RGB(31, 31, 30), RGB(12, 12, 16)};
static const u8 sCaptionColors[3] = {1, 2, 3};

enum {
    STATE_LOAD,
    STATE_FADE_IN,
    STATE_TEXT,
    STATE_HOLD,
    STATE_FADE_OUT,
    STATE_END,
};

#define tState    data[0]
#define tPanel    data[1]
#define tTimer    data[2]
#define tPanX     data[3]
#define tBolt     data[4]

static void CB2_HnsIntro(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    RunTextPrinters();
    UpdatePaletteFade();
}

static void VBlankCB_HnsIntro(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void LoadPanel(u32 i)
{
    const struct HnsPanel *p = &sPanels[i];

    LoadBgTiles(3, p->tiles, p->tilesSize, 0);
    LoadBgTilemap(3, p->map, 32 * 32 * 2, 0);
    LoadPalette(p->pal, BG_PLTT_ID(0), p->palSize);
    FillWindowPixelBuffer(WIN_CAPTION, PIXEL_FILL(1));
    PutWindowTilemap(WIN_CAPTION);
    CopyWindowToVram(WIN_CAPTION, COPYWIN_FULL);
    FillWindowPixelBuffer(WIN_TOPBAR, PIXEL_FILL(1));
    PutWindowTilemap(WIN_TOPBAR);
    CopyWindowToVram(WIN_TOPBAR, COPYWIN_FULL);
    SetGpuReg(REG_OFFSET_BG3HOFS, 0);
    SetGpuReg(REG_OFFSET_BG3VOFS, 0);
}

static void Task_HnsIntro(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    const struct HnsPanel *p = &sPanels[tPanel];

    if (JOY_NEW(START_BUTTON) && tState != STATE_END && tState != STATE_FADE_OUT)
    {
        tPanel = ARRAY_COUNT(sPanels) - 1;
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        tState = STATE_FADE_OUT;
        return;
    }

    // slow pan on every state while the panel is up
    if (p->panSpeed != 0 && (++tTimer % 8) == 0 && tState != STATE_LOAD)
    {
        tPanX += p->panSpeed;
        if (tPanX < 0)
            tPanX = 0;
        if (tPanX > 16)
            tPanX = 16;
        SetGpuReg(REG_OFFSET_BG3HOFS, p->panSpeed > 0 ? tPanX : 16 - tPanX);
    }

    switch (tState)
    {
    case STATE_LOAD:
        LoadPanel(tPanel);
        tPanX = 0;
        tTimer = 0;
        tBolt = 0;
        SetGpuReg(REG_OFFSET_BG3HOFS, p->panSpeed < 0 ? 16 : 0);
        BeginNormalPaletteFade(PALETTES_ALL, 2, 16, 0, RGB_BLACK);
        tState = STATE_FADE_IN;
        break;
    case STATE_FADE_IN:
        if (!gPaletteFade.active)
        {
            AddTextPrinterParameterized4(WIN_CAPTION, FONT_NORMAL, 12, 2, 0, 0, sCaptionColors, 2, p->caption);
            tState = STATE_TEXT;
            tTimer = 0;
        }
        break;
    case STATE_TEXT:
        if (p->lightning && tBolt == 0 && tTimer > 20)
        {
            // the lightning that takes Blaine: white flash, thunder, and a shaking frame
            tBolt = 1;
            PlaySE(SE_THUNDER);
            BeginNormalPaletteFade(PALETTES_BG, 0, 16, 0, RGB_WHITE);
        }
        if (tBolt > 0 && tBolt < 24)
        {
            tBolt++;
            SetGpuReg(REG_OFFSET_BG3VOFS, (tBolt & 2) ? 2 : -2);
            if (tBolt == 24)
                SetGpuReg(REG_OFFSET_BG3VOFS, 0);
        }
        if (!IsTextPrinterActiveOnWindow(WIN_CAPTION))
        {
            tState = STATE_HOLD;
            data[5] = 0;
        }
        break;
    case STATE_HOLD:
        if (++data[5] > 200 || (data[5] > 10 && JOY_NEW(A_BUTTON | B_BUTTON)))
        {
            BeginNormalPaletteFade(PALETTES_ALL, 2, 0, 16, RGB_BLACK);
            tState = STATE_FADE_OUT;
        }
        break;
    case STATE_FADE_OUT:
        if (!gPaletteFade.active)
        {
            if (++tPanel >= (s16)ARRAY_COUNT(sPanels))
            {
                tState = STATE_END;
            }
            else
            {
                tState = STATE_LOAD;
            }
        }
        break;
    case STATE_END:
        FreeAllWindowBuffers();
        DestroyTask(taskId);
        SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
        break;
    }
}

static void CB2_InitHnsIntro(void)
{
    SetVBlankCallback(NULL);
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    DmaFill16(3, 0, VRAM, VRAM_SIZE);
    DmaFill32(3, 0, OAM, OAM_SIZE);
    DmaFill16(3, 0, PLTT, PLTT_SIZE);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
    InitWindows(sWindowTemplates);
    DeactivateAllTextPrinters();
    ScanlineEffect_Stop();
    ResetTasks();
    ResetSpriteData();
    ResetPaletteFade();
    FreeAllSpritePalettes();
    LoadPalette(sCaptionPal, BG_PLTT_ID(15), sizeof(sCaptionPal));
    BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_BG0_ON | DISPCNT_BG3_ON);
    ShowBg(0);
    ShowBg(3);
    EnableInterrupts(INTR_FLAG_VBLANK);
    SetVBlankCallback(VBlankCB_HnsIntro);
    gTextFlags.canABSpeedUpPrint = TRUE;
    PlayBGM(MUS_HG_ROCKET_TAKEOVER);
    CreateTask(Task_HnsIntro, 0);
    SetMainCallback2(CB2_HnsIntro);
}

// special HnsPlayIntroCinematic (data/specials.inc), followed by waitstate in the script.
void HnsPlayIntroCinematic(void)
{
    SetMainCallback2(CB2_InitHnsIntro);
}
