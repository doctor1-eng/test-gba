// Heart & Soul - pixel-art cinematics (storyboard engine).
//
// Full-screen painted panels (tools/hns_intro/, asset table generated into
// src/data/hns_cinematic_panels.h) played as a small storyboard program:
//   CIN_STEP_PANEL      show a panel (fade) with a caption; same panel twice = new caption only
//   CIN_STEP_PANEL_IF   pick 1 of 2 panels/captions from a flag (branch on an earlier choice)
//   CIN_STEP_PANEL_IF2  pick 1 of 4 panels/captions from two flags
//   CIN_STEP_CHOICE     prompt + 2-option menu over the current panel; sets a flag and adds
//                       reputation (VAR_REPUTATION) for the chosen option
// Called from scripts as:
//     setvar VAR_0x8004, HNS_CINEMATIC_xxx   (include/constants/hns_cinematic.h)
//     setvar VAR_0x8005, 0 or 1              (1 = return to the overworld with the screen still
//                                             black, for a script that warps right after)
//     special HnsPlayCinematic
//     waitstate
// A/B: next caption. START: skip ahead to the next choice (never skips a decision).
//
// VRAM: BG3 = picture, 8bpp tiles in char blocks 0-2 (<= 640 tiles), map in screen block 30.
// BG0 = letterbox/caption/choice windows, 4bpp, char block 3, map in screen block 31. Picture
// palette = BG palette entries 0..223; windows use 4bpp palette slot 15.
#include "global.h"
#include "bg.h"
#include "decompress.h"
#include "event_data.h"
#include "field_screen_effect.h"
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
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "trig.h"
#include "window.h"
#include "constants/flags.h"
#include "constants/hns_cinematic.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/vars.h"

struct HnsCinPanelData
{
    const u32 *tiles;   // LZ77
    const u32 *map;     // LZ77, 32x32 u16
    const u16 *pal;
    u32 palSize;
};
#include "data/hns_cinematic_panels.h"

enum {
    CIN_STEP_PANEL,
    CIN_STEP_PANEL_IF,
    CIN_STEP_PANEL_IF2,
    CIN_STEP_CHOICE,
    CIN_STEP_END,
};

#define FX_NONE      0
#define FX_PAN_RIGHT (1 << 0)
#define FX_PAN_LEFT  (1 << 1)
#define FX_LIGHTNING (1 << 2)
#define FX_STORM     (1 << 3)

struct CinStep
{
    u8 op;
    u8 fx;
    u16 flagA;
    u16 flagB;
    u8 panels[4];
    const u8 *texts[4];
    const u8 *options[2];
    u16 setFlags[2];
    s8 rep[2];
};

#define PANEL(p, t, f) {.op = CIN_STEP_PANEL, .fx = f, .panels = {p}, .texts = {t}}
// PANEL_IF(flag, panel if flag clear, text, panel if flag set, text)
#define PANEL_IF(fl, p0, t0, p1, t1, f) {.op = CIN_STEP_PANEL_IF, .fx = f, .flagA = fl, .panels = {p0, p1}, .texts = {t0, t1}}
// PANEL_IF2(flagA, flagB, ...) variant index = (flagA set) | (flagB set) << 1
#define PANEL_IF2(fa, fb, p0, t0, p1, t1, p2, t2, p3, t3, f) {.op = CIN_STEP_PANEL_IF2, .fx = f, .flagA = fa, .flagB = fb, \
    .panels = {p0, p1, p2, p3}, .texts = {t0, t1, t2, t3}}
#define CHOICE(prompt, oA, oB, flA, flB, rA, rB) {.op = CIN_STEP_CHOICE, .texts = {prompt}, .options = {oA, oB}, \
    .setFlags = {flA, flB}, .rep = {rA, rB}}
#define END_STEP {.op = CIN_STEP_END}

// ---------------------------------------------------------------------------------------------
// Prologue: the night Team Rocket lands (played at game start, before the team is chosen).
// ---------------------------------------------------------------------------------------------
static const u8 sP1[] = _("Île de CINNABAR. Minuit.\nLa ville dort au pied du volcan.");
static const u8 sP2[] = _("Au large, des navires sans pavillon.\nPuis, un à un, des R rouges s'allument.");
static const u8 sP3[] = _("BLUE: “La LIGUE n'a jamais protégé\nque son image. Ce soir, on le prouve.”");
static const u8 sP4[] = _("En quelques minutes, le port tombe.\nLe CENTRE POKéMON est encerclé.");
static const u8 sP5[] = _("HEART & SOUL\nActe I - La chute de CINNABAR");

static const struct CinStep sSeqPrologue[] =
{
    PANEL(CIN_PANEL1, sP1, FX_PAN_RIGHT),
    PANEL(CIN_PANEL2, sP2, FX_PAN_LEFT),
    PANEL(CIN_PANEL3, sP3, FX_PAN_RIGHT),
    PANEL(CIN_PANEL4, sP4, FX_LIGHTNING),
    PANEL(CIN_PANEL5, sP5, FX_NONE),
    END_STEP,
};

// ---------------------------------------------------------------------------------------------
// La fuite: everything between the 2nd Grunt and the arrival in Pallet Town, with the three
// Act I choices played inside the cinematic and every consequence shown on screen.
// ---------------------------------------------------------------------------------------------
static const u8 sF01a[] = _("Dans le chaos, deux bâtiments ont\nencore besoin de toi avant de fuir.");
static const u8 sF01q[] = _("Le CENTRE POKéMON… ou ta propre ARÈNE ?");
static const u8 sF01oA[] = _("Sécuriser le CENTRE POKéMON");
static const u8 sF01oB[] = _("Sécuriser ton ARÈNE");
static const u8 sF02a[] = _("Avec LEVEINARD, tu évacues les blessés\ndu CENTRE POKéMON, un par un.");
static const u8 sF02b[] = _("Tu arraches aux flammes ce qui prouve\nton titre de {CHAMPION}.");
static const u8 sF03a[] = _("Un GRUNT te double en courant et laisse\ntomber un carnet de codes radio.");
static const u8 sF03b[] = _("Tu le ramasses sans savoir à quoi il\nsert. Pas le moment de t'attarder.");
static const u8 sF04a[] = _("Dans les grottes du MONT CINNABAR,\ndes habitants se cachent encore.");
static const u8 sF04q[] = _("Ils te supplient de décider pour eux.");
static const u8 sF04oA[] = _("Les guider vers un bateau");
static const u8 sF04oB[] = _("Les cacher sur place");
static const u8 sF05a[] = _("Tu les caches au fond des grottes.\nRien ne garantit qu'ils y seront encore…");
static const u8 sF05b[] = _("Tu les guides jusqu'à une barque.\nIls s'éloignent du port, vivants.");
static const u8 sF06a[] = _("Sur le quai, un CANINOS sauvage,\nblessé par les combats.");
static const u8 sF06q[] = _("Tu as le temps de l'aider… ou de fuir.");
static const u8 sF06oA[] = _("L'aider");
static const u8 sF06oB[] = _("Continuer ta course");
static const u8 sF07a[] = _("Pas le temps. Tu le laisses\nderrière toi, dans la fumée.");
static const u8 sF07b[] = _("Tu le soignes. Méfiant, il te suit\njusqu'au port.");
static const u8 sF08a[] = _("En courant, tu trébuches sur un\nbracelet gravé, tombé d'un sac.");
static const u8 sF08b[] = _("Tu le glisses dans ta poche. Il faudra\nle rendre à quelqu'un, un jour.");
static const u8 sF09_0[] = _("Le dernier bateau quitte CINNABAR.\nDerrière toi, ta ville brûle.");
static const u8 sF09_1[] = _("Devant, la lanterne de la barque des\nréfugiés. Eux, au moins, s'en sortent.");
static const u8 sF09_2[] = _("CANINOS s'est couché à tes pieds.\nDerrière vous, CINNABAR brûle.");
static const u8 sF09_3[] = _("Devant, la barque des réfugiés.\nÀ tes pieds, CANINOS. Pas tout perdu.");
static const u8 sF10[] = _("La traversée est à peine commencée\nqu'une tempête s'abat sur la mer.");
static const u8 sF11a[] = _("MARIN: Accroche-toi ! On force\nl'accostage, tant pis pour la coque !");
static const u8 sF11b[] = _("Tu ne connais même pas son nom.\nTu ne l'oublieras pas.");
static const u8 sF12_0[] = _("À l'aube, une plage. BOURG PALETTE.\nPersonne ici ne sait encore.");
static const u8 sF12_1[] = _("À l'aube, BOURG PALETTE. Au premier\nbruit, CANINOS file vers le nord.");
static const u8 sF12c[] = _("Personne ici ne sait ce qui s'est\npassé cette nuit. Pas encore.");

static const struct CinStep sSeqFuite[] =
{
    PANEL(CIN_F01, sF01a, FX_PAN_RIGHT),
    CHOICE(sF01q, sF01oA, sF01oB, 0, FLAG_SAUVETAGE_ARENE, 1, 0),
    PANEL_IF(FLAG_SAUVETAGE_ARENE, CIN_F02A, sF02a, CIN_F02B, sF02b, FX_NONE),
    PANEL(CIN_F03, sF03a, FX_PAN_LEFT),
    PANEL(CIN_F03, sF03b, FX_PAN_LEFT),
    PANEL(CIN_F04, sF04a, FX_NONE),
    CHOICE(sF04q, sF04oA, sF04oB, FLAG_REFUGIES_GUIDES, FLAG_REFUGIES_CACHES, 2, 1),
    PANEL_IF(FLAG_REFUGIES_GUIDES, CIN_F05B, sF05a, CIN_F05A, sF05b, FX_NONE),
    PANEL(CIN_F06, sF06a, FX_NONE),
    CHOICE(sF06q, sF06oA, sF06oB, FLAG_POKEMON_BLESSE_SAUVE, 0, 1, 0),
    PANEL_IF(FLAG_POKEMON_BLESSE_SAUVE, CIN_F07B, sF07a, CIN_F07A, sF07b, FX_NONE),
    PANEL(CIN_F08, sF08a, FX_NONE),
    PANEL(CIN_F08, sF08b, FX_NONE),
    PANEL_IF2(FLAG_REFUGIES_GUIDES, FLAG_POKEMON_BLESSE_SAUVE,
              CIN_F09_00, sF09_0, CIN_F09_01, sF09_1, CIN_F09_10, sF09_2, CIN_F09_11, sF09_3, FX_PAN_RIGHT),
    PANEL(CIN_F10, sF10, FX_STORM | FX_LIGHTNING),
    PANEL(CIN_F11, sF11a, FX_STORM),
    PANEL(CIN_F11, sF11b, FX_STORM),
    PANEL_IF(FLAG_POKEMON_BLESSE_SAUVE, CIN_F12_0, sF12_0, CIN_F12_1, sF12_1, FX_PAN_LEFT),
    PANEL_IF(FLAG_POKEMON_BLESSE_SAUVE, CIN_F12_0, sF12c, CIN_F12_1, sF12c, FX_PAN_LEFT),
    END_STEP,
};

static const struct CinStep *const sSequences[HNS_CINEMATIC_COUNT] =
{
    [HNS_CINEMATIC_PROLOGUE] = sSeqPrologue,
    [HNS_CINEMATIC_FUITE] = sSeqFuite,
};

static const u16 sSequenceMusic[HNS_CINEMATIC_COUNT] =
{
    [HNS_CINEMATIC_PROLOGUE] = MUS_HG_ROCKET_TAKEOVER,
    [HNS_CINEMATIC_FUITE] = MUS_HG_ENCOUNTER_ROCKET,
};

// ---------------------------------------------------------------------------------------------
// Screen setup
// ---------------------------------------------------------------------------------------------
static const struct BgTemplate sBgTemplates[] =
{
    {.bg = 0, .charBaseIndex = 3, .mapBaseIndex = 31, .screenSize = 0, .paletteMode = 0, .priority = 0, .baseTile = 0},
    {.bg = 3, .charBaseIndex = 0, .mapBaseIndex = 30, .screenSize = 0, .paletteMode = 1, .priority = 3, .baseTile = 0},
};

#define WIN_CAPTION 0
#define WIN_TOPBAR  1
#define WIN_CHOICE  2
static const struct WindowTemplate sWindowTemplates[] =
{
    [WIN_CAPTION] = {.bg = 0, .tilemapLeft = 0, .tilemapTop = 16, .width = 30, .height = 4, .paletteNum = 15, .baseBlock = 1},
    [WIN_TOPBAR]  = {.bg = 0, .tilemapLeft = 0, .tilemapTop = 0, .width = 30, .height = 1, .paletteNum = 15, .baseBlock = 121},
    [WIN_CHOICE]  = {.bg = 0, .tilemapLeft = 4, .tilemapTop = 10, .width = 22, .height = 5, .paletteNum = 15, .baseBlock = 151},
    DUMMY_WIN_TEMPLATE,
};

// window palette (slot 15): 0 transparent, 1 black, 2 white, 3 grey shadow, 4 gold (cursor)
static const u16 sWinPal[16] = {RGB_BLACK, RGB(1, 1, 3), RGB(31, 31, 30), RGB(12, 12, 16), RGB(30, 24, 8), RGB(8, 8, 12)};
static const u8 sTextColors[3] = {1, 2, 3};
static const u8 sCursorColors[3] = {1, 4, 3};
static const u8 sCursor[] = _("▶");

enum {
    ST_NEXT_STEP,
    ST_FADE_IN,
    ST_PRINT,
    ST_HOLD,
    ST_CHOICE_PRINT,
    ST_CHOICE_INPUT,
    ST_SWAP,
    ST_FADE_OUT,
    ST_EXIT,
};

struct CinState
{
    const struct CinStep *seq;
    u8 *tileBuf;
    u16 *mapBuf;
    u16 step;
    u8 state;
    u8 curPanel;      // 0xFF = nothing loaded
    u8 cursor;
    u8 stayBlack;
    u8 fx;
    u16 timer;
    u16 hold;
    u16 bolt;
    s16 panX;
    const u8 *text;
};

static EWRAM_DATA struct CinState *sCin = NULL;

static void CB2_Cinematic(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    RunTextPrinters();
    UpdatePaletteFade();
}

static void VBlankCB_Cinematic(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void ClearWindow(u8 win, bool8 show)
{
    FillWindowPixelBuffer(win, PIXEL_FILL(1));
    if (show)
        PutWindowTilemap(win);
    else
        ClearWindowTilemap(win);
    CopyWindowToVram(win, COPYWIN_FULL);
}

static void LoadCinPanel(u8 id)
{
    const struct HnsCinPanelData *p = &sCinPanels[id];

    DecompressDataWithHeaderWram(p->tiles, sCin->tileBuf);
    DecompressDataWithHeaderWram(p->map, sCin->mapBuf);
    LoadBgTiles(3, sCin->tileBuf, 640 * 64, 0);
    LoadBgTilemap(3, sCin->mapBuf, 32 * 32 * 2, 0);
    LoadPalette(p->pal, BG_PLTT_ID(0), p->palSize);
    sCin->curPanel = id;
    sCin->panX = 0;
    sCin->bolt = 0;
    SetGpuReg(REG_OFFSET_BG3HOFS, (sCin->fx & FX_PAN_LEFT) ? 16 : 0);
    SetGpuReg(REG_OFFSET_BG3VOFS, 0);
}

static void PrintCaption(const u8 *text)
{
    ClearWindow(WIN_CAPTION, TRUE);
    StringExpandPlaceholders(gStringVar4, text);
    AddTextPrinterParameterized4(WIN_CAPTION, FONT_NORMAL, 12, 2, 0, 0, sTextColors, 2, gStringVar4);
}

static void DrawChoice(const struct CinStep *s)
{
    u32 i;

    ClearWindow(WIN_CHOICE, TRUE);
    for (i = 0; i < 2; i++)
    {
        if (i == sCin->cursor)
            AddTextPrinterParameterized4(WIN_CHOICE, FONT_NORMAL, 4, 4 + i * 16, 0, 0, sCursorColors, TEXT_SKIP_DRAW, sCursor);
        AddTextPrinterParameterized4(WIN_CHOICE, FONT_NORMAL, 16, 4 + i * 16, 0, 0,
                                     i == sCin->cursor ? sCursorColors : sTextColors, TEXT_SKIP_DRAW, s->options[i]);
    }
    CopyWindowToVram(WIN_CHOICE, COPYWIN_GFX);
}

// Resolves the step's panel/caption variant from flags.
static void ResolveStep(const struct CinStep *s, u8 *panel, const u8 **text)
{
    u32 v = 0;

    if (s->op == CIN_STEP_PANEL_IF)
        v = FlagGet(s->flagA) ? 1 : 0;
    else if (s->op == CIN_STEP_PANEL_IF2)
        v = (FlagGet(s->flagA) ? 1 : 0) | (FlagGet(s->flagB) ? 2 : 0);
    *panel = s->panels[v];
    *text = s->texts[v];
}

static void UpdateFx(void)
{
    u8 fx = sCin->fx;

    sCin->timer++;
    if ((fx & (FX_PAN_RIGHT | FX_PAN_LEFT)) && (sCin->timer % 8) == 0 && sCin->panX < 16)
    {
        sCin->panX++;
        SetGpuReg(REG_OFFSET_BG3HOFS, (fx & FX_PAN_RIGHT) ? sCin->panX : 16 - sCin->panX);
    }
    if (fx & FX_STORM) // the boat rolls: slow vertical sway
        SetGpuReg(REG_OFFSET_BG3VOFS, Sin((sCin->timer * 3) & 0xFF, 4));
    if ((fx & FX_LIGHTNING) && sCin->bolt == 0 && sCin->timer > 40)
    {
        sCin->bolt = 1;
        PlaySE(SE_THUNDER);
        BeginNormalPaletteFade(PALETTES_BG & ~(1 << 15), 0, 16, 0, RGB_WHITE);
    }
    if (sCin->bolt > 0 && sCin->bolt < 24)
    {
        sCin->bolt++;
        SetGpuReg(REG_OFFSET_BG3VOFS, (sCin->bolt & 2) ? 2 : -2);
        if (sCin->bolt == 24 && !(fx & FX_STORM))
            SetGpuReg(REG_OFFSET_BG3VOFS, 0);
    }
}

static u16 FindNextChoiceOrEnd(u16 from)
{
    while (sCin->seq[from].op != CIN_STEP_CHOICE && sCin->seq[from].op != CIN_STEP_END)
        from++;
    return from;
}

static void Task_Cinematic(u8 taskId)
{
    const struct CinStep *s = &sCin->seq[sCin->step];
    u8 panel;
    const u8 *text;

    if (sCin->curPanel != 0xFF && sCin->state != ST_SWAP)
        UpdateFx();

    // START: jump to the next decision (or the end), never past a choice
    if (JOY_NEW(START_BUTTON) && (sCin->state == ST_PRINT || sCin->state == ST_HOLD))
    {
        sCin->step = FindNextChoiceOrEnd(sCin->step + 1);
        if (sCin->seq[sCin->step].op == CIN_STEP_END)
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            sCin->state = ST_FADE_OUT;
        }
        else
        {
            sCin->state = ST_NEXT_STEP;
        }
        return;
    }

    switch (sCin->state)
    {
    case ST_NEXT_STEP:
        if (s->op == CIN_STEP_END)
        {
            BeginNormalPaletteFade(PALETTES_ALL, 2, 0, 16, RGB_BLACK);
            sCin->state = ST_FADE_OUT;
            break;
        }
        if (s->op == CIN_STEP_CHOICE)
        {
            sCin->cursor = 0;
            PrintCaption(s->texts[0]);
            sCin->state = ST_CHOICE_PRINT;
            break;
        }
        ResolveStep(s, &panel, &text);
        sCin->text = text;
        if (panel == sCin->curPanel)
        {
            PrintCaption(text); // same picture: only the caption changes
            sCin->state = ST_PRINT;
        }
        else if (sCin->curPanel == 0xFF)
        {
            sCin->state = ST_SWAP; // screen is already black
        }
        else
        {
            BeginNormalPaletteFade(PALETTES_ALL, 2, 0, 16, RGB_BLACK);
            sCin->state = ST_SWAP;
        }
        break;
    case ST_SWAP:
        if (!gPaletteFade.active)
        {
            ResolveStep(s, &panel, &text);
            sCin->fx = s->fx;
            sCin->timer = 0;
            LoadCinPanel(panel);
            ClearWindow(WIN_CAPTION, TRUE);
            ClearWindow(WIN_CHOICE, FALSE);
            BeginNormalPaletteFade(PALETTES_ALL, 2, 16, 0, RGB_BLACK);
            sCin->state = ST_FADE_IN;
        }
        break;
    case ST_FADE_IN:
        if (!gPaletteFade.active)
        {
            PrintCaption(sCin->text);
            sCin->state = ST_PRINT;
        }
        break;
    case ST_PRINT:
        if (!IsTextPrinterActiveOnWindow(WIN_CAPTION))
        {
            sCin->hold = 0;
            sCin->state = ST_HOLD;
        }
        break;
    case ST_HOLD:
        if (++sCin->hold > 240 || (sCin->hold > 8 && JOY_NEW(A_BUTTON | B_BUTTON)))
        {
            sCin->step++;
            sCin->state = ST_NEXT_STEP;
        }
        break;
    case ST_CHOICE_PRINT:
        if (!IsTextPrinterActiveOnWindow(WIN_CAPTION))
        {
            DrawChoice(s);
            sCin->state = ST_CHOICE_INPUT;
        }
        break;
    case ST_CHOICE_INPUT:
        if (JOY_NEW(DPAD_UP | DPAD_DOWN))
        {
            PlaySE(SE_SELECT);
            sCin->cursor ^= 1;
            DrawChoice(s);
        }
        else if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            if (s->setFlags[sCin->cursor] != 0)
                FlagSet(s->setFlags[sCin->cursor]);
            if (s->rep[sCin->cursor] != 0)
                VarSet(VAR_REPUTATION, VarGet(VAR_REPUTATION) + s->rep[sCin->cursor]);
            ClearWindow(WIN_CHOICE, FALSE);
            sCin->step++;
            sCin->state = ST_NEXT_STEP;
        }
        break;
    case ST_FADE_OUT:
        if (!gPaletteFade.active)
            sCin->state = ST_EXIT;
        break;
    case ST_EXIT:
    {
        bool8 stayBlack = sCin->stayBlack;

        Free(sCin->tileBuf);
        Free(sCin->mapBuf);
        FREE_AND_SET_NULL(sCin);
        FreeAllWindowBuffers();
        DestroyTask(taskId);
        if (stayBlack)
            SetMainCallback2(CB2_ReturnToFieldContinueScriptStayBlack);
        else
            SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
        break;
    }
    }
}

// Return to the overworld without fading the map in: the calling script warps right away, so
// the player never sees the map flash between the cinematic and the destination.
void FieldCB_ContinueScriptStayBlack(void)
{
    LockPlayerFieldControls();
    BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 16, RGB_BLACK);
    ScriptContext_Enable();
}

static void CB2_InitCinematic(void)
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
    LoadPalette(sWinPal, BG_PLTT_ID(15), sizeof(sWinPal));
    BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
    ClearWindow(WIN_TOPBAR, TRUE);
    ClearWindow(WIN_CAPTION, TRUE);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_BG0_ON | DISPCNT_BG3_ON);
    ShowBg(0);
    ShowBg(3);
    EnableInterrupts(INTR_FLAG_VBLANK);
    SetVBlankCallback(VBlankCB_Cinematic);
    gTextFlags.canABSpeedUpPrint = TRUE;
    CreateTask(Task_Cinematic, 0);
    SetMainCallback2(CB2_Cinematic);
}

// special HnsPlayCinematic: VAR_0x8004 = HNS_CINEMATIC_*, VAR_0x8005 = 1 to come back with a
// black screen (the calling script warps immediately). Follow with waitstate.
void HnsPlayCinematic(void)
{
    u16 id = gSpecialVar_0x8004;

    if (id >= HNS_CINEMATIC_COUNT)
        id = HNS_CINEMATIC_PROLOGUE;
    sCin = AllocZeroed(sizeof(*sCin));
    sCin->seq = sSequences[id];
    sCin->tileBuf = Alloc(640 * 64);
    sCin->mapBuf = Alloc(32 * 32 * 2);
    sCin->curPanel = 0xFF;
    sCin->stayBlack = gSpecialVar_0x8005;
    PlayBGM(sSequenceMusic[id]);
    SetMainCallback2(CB2_InitCinematic);
}
