#include "config/config_debug.h"
#include "game/fb_effects.h"

// #define DEBUG_TRIGGER_IMMEDIATELY
// #define DEBUG_DONT_BLOCK_A_PRESS_FOR_FIRST
// #define DEBUG_OVERRIDE_SCORE S_BOWSER
// #define DEBUG_ALWAYS_CALCULATE_SCORE
// #define DEBUG_JUKEBOX
// #define DEBUG_TURN_ON_CS CUTSCENE_AGLAB_MAIN_SHOWCASE
// #define DEBUG_WARP_TO 0xc

extern void *load_segment_decompress_skybox(u32 segment, u8 *srcStart, u8 *srcEnd);

extern Lights1 mario_blue_lights;
extern Lights1 mario_red_lights;
extern Lights1 mario_button_v4_lights;
extern Lights1 mario_face_0___eye_open_v4_lights;
extern Lights1 mario_skin_lights;
extern Lights1 mario_mustache_v4_lights;
extern Lights1 mario_hair_001_v4_lights;
extern Lights1 mario_sideburns_v4_lights;
extern Lights1 mario_logo_lights;
extern Lights1 mario_face_1___eye_half_v4_lights;
extern Lights1 mario_face_2___eye_closed_v4_lights;
extern Lights1 mario_face_7___eye_X_v4_lights;
extern Lights1 mario_gloves_v4_lights;
extern Lights1 mario_red_dark_lights;
extern Lights1 mario_shoes_v4_lights;

u8 gMarioSounds = 0;

extern u8 begin_background_music_fade(u16 fadeDuration);
extern void seq_player_play_sequence(u8 player, u8 seqId, u16 arg2);

struct SelectedStates
{
    u8 surroundings;
    u8 castle;
    u8 pool;
    u8 bridge;
    u8 towers;
    u8 window;
    u8 frame;
    u8 roof;
    u8 main;
    u8 bg;
};

static struct SelectedStates gStates = { 0 };

static int sMusicNext = 0;
static u8 sMusicPool[] = { 0x23, 0x25, 0x26, 0x27 };

static u8 get_next_music()
{
    if (0 == sMusicNext)
    {
        // shuffle
        sMusicPool[0] = 0x23;
        sMusicPool[1] = 0x25;
        sMusicPool[2] = 0x26;
        sMusicPool[3] = 0x27;

        for (int i  = 0; i < 12; i++)
        {
            int idx0 = random_u16() & 3;
            int idx1 = random_u16() & 3;
            u8 tmp = sMusicPool[idx0];
            sMusicPool[idx0] = sMusicPool[idx1];
            sMusicPool[idx1] = tmp;
        }

        sMusicNext++;
        return sMusicPool[0];
    }
    else
    {
        int idx = sMusicNext++;
        if (idx == 3)
        {
            sMusicNext = 0;
        }

        return sMusicPool[idx];
    }
}

static void despawn_all_objects_with_behavior(const BehaviorScript *behavior) {
    uintptr_t *behaviorAddr = segmented_to_virtual(behavior);
    struct ObjectNode *listHead = &gObjectLists[get_object_list_from_behavior(behaviorAddr)];
    struct Object *obj = (struct Object *) listHead->next;
    while (obj != (struct Object *) listHead) {
        if (obj->behavior == behaviorAddr) {
            obj->activeFlags = 0;
        }

        obj = (struct Object *) obj->header.next;
    }
}

static void reset_mao()
{
    Lights1* light;

    light = (Lights1*) segmented_to_virtual(&mario_blue_lights);
    *light = (Lights1) gdSPDefLights1(
        0xD, 0x6, 0x43,
        0x0, 0x6E, 0xFF, 0x3F, 0x59, 0x3F);

    light = (Lights1*) segmented_to_virtual(&mario_red_lights);
    *light = (Lights1) gdSPDefLights1(
        0x5C, 0x0, 0x23,
        0xC9, 0x0, 0x0, 0x3F, 0x59, 0x3F);

    light = (Lights1*) segmented_to_virtual(&mario_button_v4_lights);
    *light = (Lights1) gdSPDefLights1(
        0x0, 0x12, 0x6D,
        0x0, 0x6E, 0xFF, 0x3F, 0x59, 0x3F);

    light = (Lights1*) segmented_to_virtual(&mario_face_0___eye_open_v4_lights);
    *light = (Lights1) gdSPDefLights1(
        0x9C, 0x66, 0x50,
        0xFE, 0xC1, 0x79, 0x3F, 0x59, 0x3F);

    light = (Lights1*) segmented_to_virtual(&mario_skin_lights);
    *light = (Lights1) gdSPDefLights1(
        0x9C, 0x66, 0x50,
        0xFE, 0xC1, 0x79, 0x3F, 0x59, 0x3F);

    light = (Lights1*) segmented_to_virtual(&mario_mustache_v4_lights);
    *light = (Lights1) gdSPDefLights1(
        0x9C, 0x66, 0x50,
        0xFE, 0xC1, 0x79, 0x3F, 0x59, 0x3F);

    light = (Lights1*) segmented_to_virtual(&mario_hair_001_v4_lights);
    *light = (Lights1) gdSPDefLights1(
        0x39, 0x2, 0x0,
        0x73, 0x5, 0x0, 0x28, 0x28, 0x28);

    light = (Lights1*) segmented_to_virtual(&mario_sideburns_v4_lights);
    *light = (Lights1) gdSPDefLights1(
        0x9C, 0x66, 0x50,
        0xFE, 0xC1, 0x79, 0x3F, 0x59, 0x3F);

    light = (Lights1*) segmented_to_virtual(&mario_logo_lights);
    *light = (Lights1) gdSPDefLights1(
        0x5C, 0x0, 0x23,
        0xC9, 0x0, 0x0, 0x3F, 0x59, 0x3F);

    light = (Lights1*) segmented_to_virtual(&mario_face_1___eye_half_v4_lights);
    *light = (Lights1) gdSPDefLights1(
        0x9C, 0x66, 0x50,
        0xFE, 0xC1, 0x79, 0x3F, 0x59, 0x3F);

    light = (Lights1*) segmented_to_virtual(&mario_face_2___eye_closed_v4_lights);
    *light = (Lights1) gdSPDefLights1(
        0x9C, 0x66, 0x50,
        0xFE, 0xC1, 0x79, 0x3F, 0x59, 0x3F);

    light = (Lights1*) segmented_to_virtual(&mario_face_7___eye_X_v4_lights);
    *light = (Lights1) gdSPDefLights1(
        0x9C, 0x66, 0x50,
        0xFE, 0xC1, 0x79, 0x3F, 0x59, 0x3F);

    light = (Lights1*) segmented_to_virtual(&mario_gloves_v4_lights);
    *light = (Lights1) gdSPDefLights1(
        0x69, 0x7B, 0x90,
        0xF3, 0xF3, 0xFF, 0x3F, 0x59, 0x3F);

    light = (Lights1*) segmented_to_virtual(&mario_red_dark_lights);
    *light = (Lights1) gdSPDefLights1(
        0x20, 0x0, 0x7,
        0x40, 0x0, 0xF, 0x28, 0x28, 0x28);

    light = (Lights1*) segmented_to_virtual(&mario_shoes_v4_lights);
    *light = (Lights1) gdSPDefLights1(
        0x7F, 0x7F, 0x7F,
        0xFE, 0xFE, 0xFE, 0x28, 0x28, 0x28);
}

static void randomize_color(Lights1* vlight)
{
    Lights1* light = (Lights1*) segmented_to_virtual(vlight);

    u8 r = random_float() * 255;
    u8 g = random_float() * 255;
    u8 b = random_float() * 255;

    u8 x = random_float() * 255;
    u8 y = random_float() * 255;
    u8 z = random_float() * 255;

    *light = (Lights1) gdSPDefLights1(
        r * random_float(), g * random_float(), b * random_float(),
        r, g, b, x, y, z);
}

static void randomize_mao()
{
    randomize_color(&mario_blue_lights);
    randomize_color(&mario_red_lights);
    randomize_color(&mario_button_v4_lights);
    randomize_color(&mario_face_0___eye_open_v4_lights);
    randomize_color(&mario_skin_lights);
    randomize_color(&mario_mustache_v4_lights);
    randomize_color(&mario_hair_001_v4_lights);
    randomize_color(&mario_sideburns_v4_lights);
    randomize_color(&mario_logo_lights);
    randomize_color(&mario_face_1___eye_half_v4_lights);
    randomize_color(&mario_face_2___eye_closed_v4_lights);
    randomize_color(&mario_face_7___eye_X_v4_lights);
    randomize_color(&mario_gloves_v4_lights);
    randomize_color(&mario_red_dark_lights);
    randomize_color(&mario_shoes_v4_lights);
}

extern const BehaviorScript bhvToadRunner[];
extern const BehaviorScript bhvAglabPeach[];
extern const BehaviorScript bhvAglabKoopa[];

void bhv_aglab_lakitu_init()
{
    o->oAglabLakituDialog = 30;
    spawn_object_relative_with_scale(CLOUD_BP_LAKITU_CLOUD, 0, 0, 0, 2.0f, o, MODEL_MIST, bhvCloud);
    reset_mao();
}

extern s16 gDialogID;

enum LakituActions
{
    LA_INIT,
    LA_ADVANCE,
    LA_JAMS_ADVANCE,
    LA_WELCOME,
    LA_SHOW_SURROUNDINGS,
    LA_SWITCH_SURROUNDINGS,
    LA_SWITCH_CASTLE,
    LA_SWITCH_POOL,
    LA_SWITCH_BRIDGE,
    LA_SWITCH_WINDOWS,
    LA_SWITCH_FRAME,
    LA_SWITCH_ROOF,
    LA_SWITCH_TOWERS,
    LA_SWITCH_MAIN,
    LA_SWITCH_BG,

    LA_SCORING,

    LA_S_VANILLA,
    LA_S_BOWSER,
    LA_S_BETA,
    LA_S_JAMS,
    LA_WHAT,

    LA_ENDING_DUALITY,
    LA_ENDING_EVOLUTION,
    LA_ENDING_DISASTER,
    LA_ENDING_SHELL,
    LA_ENDING_ONE,
    LA_ENDING_JAMIFY,
};

extern s16 gCutsceneTimer;
static u8 gBurnToads;

static void show_cs_time_advance(int cs, int deadline, int dialog)
{
    if (o->oTimer > deadline)
    {
        gCamera->cutscene = cs;
    }

    if (gDialogID == DIALOG_NONE)
    {
        gCamera->cutscene = cs;
        o->oAglabLakituDialog = dialog;
        o->oAglabLakituNextAction = o->oAction + 1;
        o->oAction = LA_ADVANCE;
    }
}

enum JamsEndings
{
    JE_DUALITY,
    JE_EVOLUTION,
    JE_DISASTER,
    JE_SHELL,
    JE_ONE,
    // the final one
    JE_JAMIFY,
};

static u8 dialog_from_je(u8 state)
{
    return 81 + state;
}

static struct
{
    u8* ptr;
    u8 val;
}
sJamEndingsVerifiers[] = 
{
    [ JE_DUALITY   ] = { &gStates.window, 0 },
    [ JE_EVOLUTION ] = { &gStates.towers, 3 },
    [ JE_DISASTER  ] = { &gStates.bridge, 0 },
    [ JE_SHELL     ] = { &gStates.pool, 1 },
    [ JE_ONE       ] = { &gStates.main, 2 },
};

static void show_cs_time_advance_jams(int cs)
{
    if (o->oTimer == 0)
    {
        gCutsceneTimer = 0;
    }

    gCamera->cutscene = cs;
    if (gDialogID == DIALOG_NONE)
    {
        for (s32 nextAction = o->oAction + 1; ; nextAction++)
        {
            s32 nextJam = nextAction - LA_ENDING_DUALITY;
            if (nextAction == LA_ENDING_JAMIFY 
            || (sJamEndingsVerifiers[nextJam].val == *sJamEndingsVerifiers[nextJam].ptr))
            { 
                o->oAglabLakituDialog = dialog_from_je(nextJam);
                o->oAglabLakituNextAction = nextAction;
                o->oAction = LA_ADVANCE;
                break;
            }
        }
    }
}

static void show_finale(int cs, int picked)
{
    gCamera->cutscene = cs;
    if (gDialogID == DIALOG_NONE)
    {
        o->oAglabLakituDialog = 50 + 10 * picked;
        o->oAglabLakituNextAction = LA_SCORING + 1 + picked;
        o->oAction = LA_ADVANCE;
    }
}

extern const Texture aglab_zargrass[];
extern const Texture aglab_zarsand[];
extern const Texture aglab_zarside[];
extern const Texture aglab_betaside[];
extern const Texture aglab_betagrass[];
extern const Texture aglab_betasand2[];
extern const Texture aglab_normalgrass[];
extern const Texture aglab_normalside[];
extern const Texture aglab_normalsand[];
extern u8 castle_grounds_dl_Shape_144_rgba16[];
extern u8 castle_grounds_dl_Shape_145_rgba16[];
extern u8 castle_grounds_dl_Shape_160_rgba16[];

extern Gfx mat_castle_grounds_dl_Shape_144_f3d[];
extern Gfx mat_castle_grounds_dl_Shape_145_f3d[];
extern Gfx mat_castle_grounds_dl_Shape_152_f3d[];
extern Gfx mat_castle_grounds_dl_Shape_160_f3d[];

static void randomize_state()
{
    u8* states = &gStates.surroundings;
    for (int i = 0; i < sizeof(gStates); i++)
    {
#ifdef DEBUG_ALWAYS_CALCULATE_SCORE
        states[i] = -1;
#else
        states[i] = (u8) random_u16();
#endif
    }
}

static const void* sSurroundingsTextures[][3] = {
    { aglab_zargrass, aglab_zarsand, aglab_zarside },
    { castle_grounds_dl_Shape_144_rgba16, castle_grounds_dl_Shape_145_rgba16, castle_grounds_dl_Shape_160_rgba16 },
    { aglab_betagrass, aglab_betasand2, aglab_betaside },
    { aglab_normalgrass, aglab_normalsand, aglab_normalside },
};

static void switch_surroundings()
{
    const void** text;

    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_144_f3d) + 2*6 + 1;
    *text = sSurroundingsTextures[gStates.surroundings][0];
    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_152_f3d) + 2*6 + 1;
    *text = sSurroundingsTextures[gStates.surroundings][0];
    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_145_f3d) + 2*6 + 1;
    *text = sSurroundingsTextures[gStates.surroundings][1];
    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_160_f3d) + 2*6 + 1;
    *text = sSurroundingsTextures[gStates.surroundings][2];
}

extern Gfx castle_grounds_dl_walls_mesh_layer_6[];
extern Gfx castle_grounds_dl_walls_mesh_layer_4[];
extern Gfx mat_castle_grounds_dl_Shape_139_f3d[];

extern u8 castle_grounds_dl_Shape_139_rgba16[];
extern const Texture aglab_betawall[];
extern const Texture aglab_zarwall[];
extern const Texture aglab_normalwall[];

static const void* sCastleTextures[] = {
    castle_grounds_dl_Shape_139_rgba16, aglab_betawall, aglab_zarwall, aglab_normalwall 
};

static void switch_castle()
{
    u8* env;
    const void** text;
    
    env = (u8*) segmented_to_virtual(castle_grounds_dl_walls_mesh_layer_6) + 15;
    *env = 0xff;
    env = (u8*) segmented_to_virtual(castle_grounds_dl_walls_mesh_layer_4) + 15;
    *env = 0xff;

    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_139_f3d) + 2*6 + 1;
    *text = sCastleTextures[gStates.castle];
}

extern Gfx mat_castle_grounds_dl_Shape_131_f3d[];
extern Gfx mat_castle_grounds_dl_Shape_136_f3d[];
extern Gfx mat_castle_grounds_dl_Shape_259_f3d[];
extern Gfx mat_castle_grounds_dl_Shape_258_f3d[];
extern u8 castle_grounds_dl_Shape_259_rgba16[];
extern u8 castle_grounds_dl_Shape_258_rgba16[];
extern const Texture aglab_zarlava[];
extern const Texture aglab_zarlava2[];
extern const Texture aglab_zarrock[];
extern const Texture aglab_jamwater[];
extern const Texture aglab_jamwater2[];
extern const Texture aglab_jamrock2[];
extern const Texture aglab_zarwalls[];
extern const Texture aglab_zarground2[];
extern const Texture aglab_jamground[];
extern const Texture aglab_jamground2[];

extern Gfx mat_castle_grounds_dl_Shape_131_f3d[];
extern Gfx mat_castle_grounds_dl_Shape_136_f3d[];
extern Gfx mat_castle_grounds_dl_Shape_155_f3d[];
extern Gfx mat_castle_grounds_dl_Shape_156_f3d[];
extern Gfx mat_castle_grounds_dl_Shape_157_f3d[];
extern Gfx mat_castle_grounds_dl_Shape_158_f3d[];
extern Gfx mat_castle_grounds_dl_Shape_159_f3d[];
extern u8 castle_grounds_dl_Shape_155_rgba16[];

extern const Texture aglab_betaground1[];
extern const Texture aglab_betaground2[];
extern const Texture aglab_betawall3[];

static const void* sWaterTextures[][5] = {
    { castle_grounds_dl_Shape_258_rgba16, castle_grounds_dl_Shape_259_rgba16, aglab_betawall3, aglab_betaground1, aglab_betaground2 },
    { aglab_jamwater2, aglab_jamwater, aglab_jamrock2, aglab_jamground, aglab_jamground2 },
    { castle_grounds_dl_Shape_258_rgba16, castle_grounds_dl_Shape_259_rgba16, castle_grounds_dl_Shape_155_rgba16, mat_castle_grounds_dl_Shape_131_f3d, mat_castle_grounds_dl_Shape_136_f3d },
    { aglab_zarlava2, aglab_zarlava, aglab_zarwalls, aglab_zarground2, aglab_zarrock },
};

static void switch_water()
{
    u8 wantEnv = gStates.pool ? 200 : 0;

    u8* env;
    const void** text;
    env = (u8*) segmented_to_virtual(mat_castle_grounds_dl_Shape_259_f3d) + 4*8 + 7;
    *env = wantEnv;
    env = (u8*) segmented_to_virtual(mat_castle_grounds_dl_Shape_258_f3d) + 4*8 + 7;
    *env = wantEnv;
    
    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_259_f3d) + 2*8 + 1;
    *text = sWaterTextures[gStates.pool][0];
    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_258_f3d) + 2*8 + 1;
    *text = sWaterTextures[gStates.pool][1];
    
    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_155_f3d) + 2*6 + 1;
    *text = sWaterTextures[gStates.pool][2];
    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_156_f3d) + 2*6 + 1;
    *text = sWaterTextures[gStates.pool][2];
    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_157_f3d) + 2*6 + 1;
    *text = sWaterTextures[gStates.pool][2];
    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_158_f3d) + 2*6 + 1;
    *text = sWaterTextures[gStates.pool][2];
    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_131_f3d) + 2*6 + 1;
    *text = sWaterTextures[gStates.pool][3];
    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_136_f3d) + 2*7 + 1;
    *text = sWaterTextures[gStates.pool][4];

    if (1 == gStates.pool)
    {
        f32 d;
        if (!cur_obj_find_nearest_object_with_behavior(bhvAglabKoopa, &d))
        {
            f32 y = -416.f;
            for (int i = 0; i < 3; i++)
            {
                {
                    struct Object* koopa = spawn_object(o, MODEL_KOOPA_WITHOUT_SHELL, bhvAglabKoopa);
                    koopa->oPosX = random_float_ft(3588.f, 4511.f);
                    koopa->oPosY = y;
                    koopa->oPosZ = random_float_ft(2835.f, 4525.f);
                    koopa->oAglabKoopaAngle = random_u16();
                    koopa->oTimer = random_float() * 140;
                    koopa->oFaceAngleYaw = koopa->oFaceAngleRoll = koopa->oFaceAnglePitch = 0;
                }
                {
                    struct Object* koopa = spawn_object(o, MODEL_KOOPA_WITHOUT_SHELL, bhvAglabKoopa);
                    koopa->oPosX = random_float_ft(3588.f, 4511.f);
                    koopa->oPosY = y;
                    koopa->oPosZ = random_float_ft(1000.f, 2835.f);
                    koopa->oAglabKoopaAngle = random_u16();
                    koopa->oTimer = random_float() * 140;
                    koopa->oFaceAngleYaw = koopa->oFaceAngleRoll = koopa->oFaceAnglePitch = 0;
                }
                {
                    struct Object* koopa = spawn_object(o, MODEL_KOOPA_WITHOUT_SHELL, bhvAglabKoopa);
                    koopa->oPosX = random_float_ft(645.f, 2466.f);
                    koopa->oPosY = y;
                    koopa->oPosZ = random_float_ft(273.f, 2286.f);
                    koopa->oAglabKoopaAngle = random_u16();
                    koopa->oTimer = random_float() * 140;
                    koopa->oFaceAngleYaw = koopa->oFaceAngleRoll = koopa->oFaceAnglePitch = 0;
                }
            }
        }
    }
    else
    {
        despawn_all_objects_with_behavior(bhvAglabKoopa);
        despawn_all_objects_with_behavior(bhvKoopaShell);
        despawn_all_objects_with_behavior(bhvWaterDroplet);
        despawn_all_objects_with_behavior(bhvObjectWaveTrail);
    }
}

extern const Texture aglab_betawwall2[];
extern const Texture aglab_hdwall[];
extern Gfx castle_grounds_dl_briodge_mesh_layer_4[];

extern u8 castle_grounds_dl_Shape_133_rgba16[];
extern u8 castle_grounds_dl_Shape_135_rgba16[];

extern Gfx mat_castle_grounds_dl_Shape_133_f3d[];
extern Gfx mat_castle_grounds_dl_Shape_135_f3d[];
extern Gfx mat_castle_grounds_dl_Shape_137_f3d[];

static const void* sBridgeTextures[][2] = {
    { aglab_jamground2, aglab_hdwall },
    { aglab_zarrock, aglab_zarwall },
    { castle_grounds_dl_Shape_133_rgba16, castle_grounds_dl_Shape_135_rgba16 },
    { aglab_betaground1, aglab_betawwall2 },
};

static void switch_bridge()
{
    if (0 == gStates.bridge)
    {
        f32 d;
        if (!cur_obj_find_nearest_object_with_behavior(bhvStaticObjectEx2, &d))
        {
            struct Object* b = spawn_object(o, MODEL_CASTLE_GROUNDS_DISASTER_BRIDGE, bhvStaticObjectEx2);
            b->oPosX = 0;
            b->oPosY = 0;
            b->oPosZ = 0;
            b->oFaceAngleYaw = 0;
            b->oFaceAnglePitch = 0;
            b->oFaceAngleRoll = 0;
        }
    }
    else
    {
        despawn_all_objects_with_behavior(bhvStaticObjectEx2);
    }

    const void** text;
    u8* env;
    u8 venv = 0 == gStates.bridge ? 0 : 0xff;
    env = (u8*) segmented_to_virtual(castle_grounds_dl_briodge_mesh_layer_4) + 15;
    *env = venv;
    
    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_133_f3d) + 2*7 + 1;
    *text = sBridgeTextures[gStates.bridge][0];
    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_135_f3d) + 2*6 + 1;
    *text = sBridgeTextures[gStates.bridge][1];
    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_137_f3d) + 2*7 + 1;
    *text = sBridgeTextures[gStates.bridge][1];
}

extern Gfx castle_grounds_dl_windows_mesh_layer_4[];
extern u8 castle_grounds_dl_Shape_231_rgba16[];
extern Gfx mat_castle_grounds_dl_Shape_231_f3d[];
extern Gfx mat_castle_grounds_dl_f3dlite_material_001[];
extern const Texture aglab_luigiwindow[];
extern const Texture aglab_mariowindow[];
extern const Texture aglab_zarwindow[];
extern const Texture aglab_betawindow[];

static const void* sWindowTextures[][2] = {
    { aglab_luigiwindow, aglab_mariowindow },
    { castle_grounds_dl_Shape_231_rgba16, castle_grounds_dl_Shape_231_rgba16 },
    { aglab_zarwindow, aglab_zarwindow },
    { aglab_betawindow, aglab_betawindow },
};

static void switch_window()
{
    const void** text;
    u8* env;

    env = (u8*) segmented_to_virtual(castle_grounds_dl_windows_mesh_layer_4) + 15;
    *env = 0xff;
    
    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_231_f3d) + 2*7 + 1;
    *text = sWindowTextures[gStates.window][0];

    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_f3dlite_material_001) + 2*7 + 1;
    *text = sWindowTextures[gStates.window][1];
}

extern Gfx castle_grounds_dl_frame_mesh_layer_4[];
extern Gfx mat_castle_grounds_dl_Shape_232_f3d[];
extern Gfx mat_castle_grounds_dl_Shape_233_f3d[];
extern const Texture aglab_bowserb[];
extern const Texture aglab_bowsert[];
extern u8 castle_grounds_dl_Shape_233_rgba16[];
extern u8 castle_grounds_dl_Shape_232_rgba16[];
extern const Texture aglab_betat[];
extern const Texture aglab_betab[];
extern const Texture aglab_jamst[];
extern const Texture aglab_jamsb[];
extern Gfx castle_grounds_dl_frame_mesh_layer_4[];

static const void* sFrameTextures[][2] = {
    { aglab_betab, aglab_betat },
    { aglab_jamst, aglab_jamsb },
    { castle_grounds_dl_Shape_233_rgba16, castle_grounds_dl_Shape_232_rgba16 },
    { aglab_bowsert, aglab_bowserb },
};

static void switch_frame()
{
    const void** text;
    u8* env;

    env = (u8*) segmented_to_virtual(castle_grounds_dl_frame_mesh_layer_4) + 15;
    *env = 0xff;
    
    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_233_f3d) + 2*6 + 1;
    *text = sFrameTextures[gStates.frame][0];

    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_232_f3d) + 2*6 + 1;
    *text = sFrameTextures[gStates.frame][1];
}

extern Gfx castle_grounds_dl_roof_mesh_layer_4[];
extern Gfx mat_castle_grounds_dl_Shape_134_f3d[];
extern u8 castle_grounds_dl_Shape_134_rgba16[];
extern const Texture aglab_hdroof[];
extern const Texture aglab_zarroof[];
extern const Texture aglab_betaroof[];
static const void* sRoofTextures[] = {
    castle_grounds_dl_Shape_134_rgba16, aglab_hdroof, aglab_zarroof, aglab_betaroof
};

static void switch_roof()
{
    const void** text;
    u8* env;

    env = (u8*) segmented_to_virtual(castle_grounds_dl_roof_mesh_layer_4) + 15;
    *env = 0xff;

    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_134_f3d) + 2*7 + 1;
    *text = sRoofTextures[gStates.roof];
}

static u8 sTowerModels[] = { MODEL_CASTLE_GROUNDS_CANNON_GRILL, MODEL_CASTLE_GROUNDS_VCUTM_GRILL, MODEL_CASTLE_GROUNDS_CANNON_GRILL, MODEL_CASTLE_GROUNDS_FLAG };

extern Gfx mat_vtowers_Shape_135_f3d[];
extern Gfx mat_vtowers_Shape_139_f3d[];
extern Gfx mat_vtowers_Shape_134_f3d[];

static void switch_towers()
{
    f32 d;
    struct Object* tower = cur_obj_find_nearest_object_with_behavior(bhvStaticObject, &d);
    if (!tower)
    {
        tower = spawn_object(o, sTowerModels[gStates.towers], bhvStaticObject);
        tower->oPosX = 0;
        tower->oPosY = 0;
        tower->oPosZ = 0;
        tower->oFaceAngleYaw = 0;
        tower->oFaceAnglePitch = 0;
        tower->oFaceAngleRoll = 0;
    }

    obj_set_model(tower, sTowerModels[gStates.towers]);

    void** text;
    text = (const void**) segmented_to_virtual(mat_vtowers_Shape_135_f3d) + 2*6 + 1;
    *text = gStates.towers ? aglab_betawwall2 : castle_grounds_dl_Shape_135_rgba16;
    text = (const void**) segmented_to_virtual(mat_vtowers_Shape_139_f3d) + 2*6 + 1;
    *text = gStates.towers ? aglab_betawall : castle_grounds_dl_Shape_139_rgba16;
    text = (const void**) segmented_to_virtual(mat_vtowers_Shape_134_f3d) + 2*7 + 1;
    *text = gStates.towers ? aglab_betaroof : castle_grounds_dl_Shape_134_rgba16;
}

extern Gfx castle_grounds_dl_tmain_mesh_layer_1[];
extern Gfx castle_grounds_dl_tmain_mesh_layer_4[];
extern Gfx mat_castle_grounds_dl_Shape_192_f3d[];
extern Gfx mat_castle_grounds_dl_Shape_193_f3d[];
extern u8 castle_grounds_dl_Shape_192_rgba16[];
extern u8 castle_grounds_dl_Shape_193_rgba16[];

static const void* sMainTextures[][2] = 
{ 
    { aglab_betaroof, aglab_betawall3 },
    { castle_grounds_dl_Shape_192_rgba16, castle_grounds_dl_Shape_193_rgba16 },
    { castle_grounds_dl_Shape_192_rgba16, castle_grounds_dl_Shape_193_rgba16 },
    { aglab_zarroof, aglab_zarwall },
};

static void switch_main()
{
    u8 venv = gStates.main == 2 ? 0 : 0xff;
    u8* env;
    env = (u8*) segmented_to_virtual(castle_grounds_dl_tmain_mesh_layer_1) + 15;
    *env = venv;
    env = (u8*) segmented_to_virtual(castle_grounds_dl_tmain_mesh_layer_4) + 15;
    *env = venv;

    const void** text;
    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_192_f3d) + 2*7 + 1;
    *text = sMainTextures[gStates.main][0];
    text = (const void**) segmented_to_virtual(mat_castle_grounds_dl_Shape_193_f3d) + 2*6 + 1;
    *text = sMainTextures[gStates.main][1];

    if (2 == gStates.main)
    {
        f32 d;
        if (!cur_obj_find_nearest_object_with_behavior(bhvStaticObjectEx, &d))
        {
            struct Object* tower = spawn_object(o, MODEL_CASTLE_GROUNDS_ONE_TOWER, bhvStaticObjectEx);
            tower->oPosX = 0;
            tower->oPosY = 0;
            tower->oPosZ = 0;
            tower->oFaceAngleYaw = 0;
            tower->oFaceAnglePitch = 0;
            tower->oFaceAngleRoll = 0;
        }
    }
    else
    {
        despawn_all_objects_with_behavior(bhvStaticObjectEx);
    }
}

extern s8 gDialogBoxState;
extern u8 gBlockDialogClosing;

static void advance_state(int num, u8* val)
{
    if (0 != o->oTimer && gDialogBoxState != 1)
    {
        return;
    }

    if (0 == o->oTimer % num)
    {
        (*val)++;
        (*val) %= 4;
    }
}

static void switch_bg()
{
    static u8 LastBg = 0;
    if (LastBg != gStates.bg)
    {
        LastBg = gStates.bg;
        switch (gStates.bg)
        {
            case 0:
                load_segment_decompress_skybox(0xA,_bidw_skybox_mio0SegmentRomStart, _bidw_skybox_mio0SegmentRomEnd);
                break;
            case 1:
                load_segment_decompress_skybox(0xA,_cloud_floor_skybox_yay0SegmentRomStart, _cloud_floor_skybox_yay0SegmentRomEnd);
                break;
            case 2:
                load_segment_decompress_skybox(0xA,_ccm_skybox_mio0SegmentRomStart, _ccm_skybox_mio0SegmentRomEnd);
                break;
            case 3:
                load_segment_decompress_skybox(0xA,_bitfs_skybox_mio0SegmentRomStart, _bitfs_skybox_mio0SegmentRomEnd);
                break;
        }
    }
}

struct Scores
{
    s8 arr[4];
};

enum ScoreTypes
{
    S_VANILLA,
    S_BOWSER,
    S_BETA,
    S_JAMS,
};

struct Scores gScores = { 0 };

static void calculate_score()
{
    // I really should refactor this jesus christ
    gScores = (struct Scores){ 0 };

    if (0 == gStates.surroundings) gScores.arr[S_BOWSER] += 2;
    if (1 == gStates.surroundings) gScores.arr[S_VANILLA] += 2;
    if (2 == gStates.surroundings) gScores.arr[S_BETA] += 1;

    if (0 == gStates.castle) gScores.arr[S_VANILLA] += 2;
    if (1 == gStates.castle) gScores.arr[S_BETA] += 2;
    if (2 == gStates.castle) gScores.arr[S_BOWSER] += 1;

    if (0 == gStates.pool) gScores.arr[S_BETA] += 1;
    if (1 == gStates.pool) gScores.arr[S_JAMS] += 3;
    if (2 == gStates.pool) gScores.arr[S_VANILLA] += 2;
    if (3 == gStates.pool) gScores.arr[S_BOWSER] += 2;

    if (2 == gStates.bridge) gScores.arr[S_VANILLA] += 1;
    if (1 == gStates.bridge) gScores.arr[S_BOWSER] += 2;
    if (3 == gStates.bridge) gScores.arr[S_BETA] += 1;
    if (0 == gStates.bridge) gScores.arr[S_JAMS] += 2;

    if (0 == gStates.window) gScores.arr[S_JAMS] += 2;
    if (1 == gStates.window) gScores.arr[S_VANILLA] += 1;
    if (2 == gStates.window) gScores.arr[S_BOWSER] += 1;

    if (0 == gStates.frame) gScores.arr[S_BETA] += 3;
    if (1 == gStates.frame) gScores.arr[S_JAMS] += 3;
    if (2 == gStates.frame) gScores.arr[S_VANILLA] += 2;
    if (3 == gStates.frame) gScores.arr[S_BOWSER] += 2;

    if (0 == gStates.roof) gScores.arr[S_VANILLA] += 1;
    if (2 == gStates.roof) gScores.arr[S_BOWSER] += 2;
    if (3 == gStates.roof) gScores.arr[S_BETA] += 2;

    if (0 == gStates.towers) gScores.arr[S_VANILLA] += 1;
    if (1 == gStates.towers) gScores.arr[S_BOWSER] += 1;
    if (2 == gStates.towers) gScores.arr[S_BETA] += 2;
    if (3 == gStates.towers) gScores.arr[S_JAMS] += 2;

    if (0 == gStates.main) gScores.arr[S_BETA] += 3;
    if (1 == gStates.main) gScores.arr[S_VANILLA] += 2;
    if (2 == gStates.main) gScores.arr[S_JAMS] += 3;
    if (3 == gStates.main) gScores.arr[S_BOWSER] += 1;

    if (3 == gStates.bg) gScores.arr[S_BOWSER] += 1;
    if (1 == gStates.bg) gScores.arr[S_VANILLA] += 1;
}

static int find_largest_score()
{
#ifdef DEBUG_OVERRIDE_SCORE
    return DEBUG_OVERRIDE_SCORE;
#endif

    int which = 0;
    int score = gScores.arr[0];
    for (int i = 1; i < 4; i++)
    {
        if (score < gScores.arr[i])
        {
            score = gScores.arr[i];
            which = i;
        }
    }

    if (gScores.arr[which] < 11)
    {
        return 4; /*invalid*/
    }

    return which;
}

extern const BehaviorScript bhvFinalBridge[];
extern const BehaviorScript bhvJam[];

u8 gWantCustomDeath = 0;
extern u8 sDialogSpeaker[];
extern s32 sDialogSpeakerVoice[];

void bhv_aglab_lakitu_loop()
{
#ifdef DEBUG_WARP_TO
    if (0 == o->oTimer)
    {
        gMarioStates->usedObj = o;
        SET_BPARAM2(o->oBehParams, DEBUG_WARP_TO);
        level_trigger_warp(gMarioStates, WARP_OP_TELEPORT);
    }
#endif

#ifdef DEBUG_TURN_ON_CS
    if (o->oTimer > 30)
        gCamera->cutscene = DEBUG_TURN_ON_CS;
#endif

#ifdef DEBUG_JUKEBOX
    static u8 sPlayingSong = 0;
    static u8 sWantSong = 0x25;

    if (gPlayer1Controller->buttonPressed & L_JPAD)
    {
        sWantSong--;
    }
    if (gPlayer1Controller->buttonPressed & R_JPAD)
    {
        sWantSong++;
    }
    
    if (sWantSong != sPlayingSong)
    {
        sPlayingSong = sWantSong;
        seq_player_play_sequence(0, sPlayingSong, 0);
    }
    print_text_fmt_int(20, 20, "%d", sPlayingSong);
#endif

    o->oHomeX = gMarioStates->pos[0];
    o->oHomeZ = gMarioStates->pos[2];

    o->oFaceAngleYaw = cur_obj_angle_to_home();
    o->oFaceAnglePitch = atan2s(cur_obj_lateral_dist_to_home(), o->oPosY - gMarioStates->pos[1]);

    // print_text_fmt_int(20, 60, "X %d", (int) gMarioStates->pos[0]);
    // print_text_fmt_int(20, 40, "Y %d", (int) gMarioStates->pos[1]);
    // print_text_fmt_int(20, 20, "Z %d", (int) gMarioStates->pos[2]);

#if 0
    print_text_fmt_int(200, 200, "A %d", o->oAction);
    print_text_fmt_int(200, 180, "T %d", o->oSubAction);
    print_text_fmt_int(200, 160, "D %d", gDialogID);
    print_text_fmt_int(200, 140, "S %d", sDialogSpeaker[gDialogID]);
    print_text_fmt_int(200, 120, "V %x", 0xffff & (sDialogSpeakerVoice[sDialogSpeaker[gDialogID]] >> 16));
#endif

#ifdef DEBUG_ALWAYS_CALCULATE_SCORE
    calculate_score();

    print_text_fmt_int(20, 80, "J %d", gScores.arr[S_JAMS]);
    print_text_fmt_int(20, 60, "B %d", gScores.arr[S_BOWSER]);
    print_text_fmt_int(20, 40, "Z %d", gScores.arr[S_BETA]);
    print_text_fmt_int(20, 20, "V %d", gScores.arr[S_VANILLA]);
#endif

    if (LA_INIT == o->oAction)
    {
        gMarioSounds = 0;
        set_fb_effect_col(255, 255, 255, 0);
#ifndef DEBUG_TRIGGER_IMMEDIATELY
        if (o->oDistanceToMario < 500.f)
#endif
        {
            if (set_mario_npc_dialog(MARIO_DIALOG_LOOK_UP) == MARIO_DIALOG_STATUS_START)
            {
                create_dialog_box(o->oAglabLakituDialog);
                o->oAction = LA_WELCOME;
            }
        }
    }
    else if (LA_ADVANCE == o->oAction)
    {
        set_mario_npc_dialog(MARIO_DIALOG_LOOK_UP);
        create_dialog_box(o->oAglabLakituDialog);
        o->oAction = o->oAglabLakituNextAction;
    }
    else if (LA_WELCOME == o->oAction)
    {
        show_cs_time_advance(CUTSCENE_AGLAB_CASTLE_VIEW, 30, 31);
    }
    else if (LA_SHOW_SURROUNDINGS == o->oAction)
    {
        show_cs_time_advance(CUTSCENE_AGLAB_OUTSIDES, 160, 32);
    }
    else if (LA_SWITCH_SURROUNDINGS == o->oAction)
    {
        if (0 == o->oTimer)
        {
            randomize_state();
            // doesnt work?
            begin_background_music_fade(50);
            seq_player_play_sequence(0, get_next_music(), 50);
        }

#ifndef DEBUG_DONT_BLOCK_A_PRESS_FOR_FIRST
        gBlockDialogClosing = o->oTimer < 30;
#endif

        advance_state(30, &gStates.surroundings);
        switch_surroundings();
        show_cs_time_advance(CUTSCENE_AGLAB_OUTSIDES, 0, 33);
    }
    else if (LA_SWITCH_CASTLE == o->oAction)
    {
        advance_state(30, &gStates.castle);
        switch_castle();
        show_cs_time_advance(CUTSCENE_AGLAB_CASTLE_WALLS, 0, 34);
    }
    else if (LA_SWITCH_POOL == o->oAction)
    {
        advance_state(30, &gStates.pool);
        switch_water();
        show_cs_time_advance(CUTSCENE_AGLAB_WATER, 0, 35);
    }
    else if (LA_SWITCH_BRIDGE == o->oAction)
    {
        advance_state(30, &gStates.bridge);
        switch_bridge();
        show_cs_time_advance(CUTSCENE_AGLAB_BRIDGE, 0, 36);
    }
    else if (LA_SWITCH_WINDOWS == o->oAction)
    {
        advance_state(30, &gStates.window);
        switch_window();
        show_cs_time_advance(CUTSCENE_AGLAB_WINDOW, 0, 37);
    }
    else if (LA_SWITCH_FRAME == o->oAction)
    {
        advance_state(30, &gStates.frame);
        switch_frame();
        show_cs_time_advance(CUTSCENE_AGLAB_FRAME, 0, 38);
    }
    else if (LA_SWITCH_ROOF == o->oAction)
    {
        advance_state(30, &gStates.roof);
        switch_roof();
        show_cs_time_advance(CUTSCENE_AGLAB_ROOF, 0, 39);
    }
    else if (LA_SWITCH_TOWERS == o->oAction)
    {
        advance_state(30, &gStates.towers);
        switch_towers();
        show_cs_time_advance(CUTSCENE_AGLAB_TOWERS, 0, 40);
    }
    else if (LA_SWITCH_MAIN == o->oAction)
    {
        advance_state(30, &gStates.main);
        switch_main();
        show_cs_time_advance(CUTSCENE_AGLAB_MAIN, 0, 41);
    }
    else if (LA_SWITCH_BG == o->oAction)
    {
        advance_state(30, &gStates.bg);
        switch_bg();
        show_cs_time_advance(CUTSCENE_AGLAB_BG, 0, 42);
    }
    else if (LA_SCORING == o->oAction)
    {
        if (0 == o->oTimer)
            seq_player_play_sequence(0, 0x24, 0);

#ifndef DEBUG_DONT_BLOCK_A_PRESS_FOR_FIRST
        gBlockDialogClosing = o->oTimer < 100;
#endif

        if (0 == o->oTimer)
        {
            gCutsceneTimer = 0;
            calculate_score();
            o->oAglabLakituChosen = find_largest_score();
        }

        show_finale(CUTSCENE_AGLAB_CASTLE_VIEW2, o->oAglabLakituChosen);
    }
    else if (LA_S_VANILLA == o->oAction)
    {
        if (0 == o->oTimer)
            seq_player_play_sequence(0, 0x1a, 0);

        gCamera->cutscene = 0;
        if (gDialogID == DIALOG_NONE && 0 == o->oSubAction)
        {
            o->oSubAction = 1;
            struct Object* p = spawn_object(o, MODEL_PEACH, bhvAglabPeach);
            p->oPosX = gMarioStates->pos[0] + 120.f * sins(gMarioStates->faceAngle[1]);
            p->oPosY = gMarioStates->pos[1];
            p->oPosZ = gMarioStates->pos[2] + 120.f * coss(gMarioStates->faceAngle[1]);
            p->oFaceAngleRoll = 0;
            p->oFaceAnglePitch = 0;
            p->oFaceAngleYaw = 0x8000 + gMarioStates->faceAngle[1];

            set_mario_animation(gMarioStates, MARIO_ANIM_CREDITS_LOOK_UP);
            set_mario_action(gMarioStates, ACT_END_PEACH_CUTSCENE, 8);
            gMarioStates->actionArg = 8;
        }
    }
    else if (LA_S_BOWSER == o->oAction)
    {
        if (0 == o->oTimer)
            seq_player_play_sequence(0, SEQ_LEVEL_BOSS_KOOPA, 0);

        if (0 == o->oTimer)
        {
            gBurnToads = 0;
            {
                struct Object* br = spawn_object(o, 0, bhvFinalBridge);
                br->oPosX = br->oPosY = br->oPosZ = 0;
                br->oFaceAngleYaw = 0;
                br->oFaceAnglePitch = 0;
                br->oFaceAngleRoll = 0;
            }
            for (int i = 0; i < 10; i++)
            {
                struct Object* toad = spawn_object(o, MODEL_TOAD, bhvToadRunner);
                toad->oPosX = random_f32_around_zero(900.f);
                toad->oPosY = 600.f;
                toad->oPosZ = 2600.f + random_f32_around_zero(400.f);
                toad->oFaceAngleYaw = 0;
                toad->oFaceAnglePitch = 0;
                toad->oFaceAngleRoll = 0;
            }
            o->oAglabLakituFriend = spawn_object(o, MODEL_BOWSER, bhvBowser);
            o->oAglabLakituFriend->oPosX = gMarioStates->pos[0] = -39.f;
            o->oAglabLakituFriend->oPosY = gMarioStates->pos[1] = 365.f;
            o->oAglabLakituFriend->oPosZ = gMarioStates->pos[2] = -399.f;
            o->oAglabLakituFriend->oFaceAngleYaw = 0x8000;
            o->oAglabLakituFriend->oFaceAnglePitch = 0;
            o->oAglabLakituFriend->oFaceAngleRoll = 0;
        }
        
        gCamera->cutscene = 0;
        gMarioObject->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;

        if (gDialogID == DIALOG_NONE)
        {
            if (0 == o->oSubAction)
            {
                o->oSubAction = 1;
                o->oAglabLakituFriend->oAction = 15;
                o->oTimer = 0;
            }

            if (50 == o->oTimer)
            {
                gMarioStates->usedObj = o;
                SET_BPARAM2(o->oBehParams, 0xc);
                level_trigger_warp(gMarioStates, WARP_OP_TELEPORT);
            }
            if (60 == o->oTimer)
            {
                gBurnToads = 1;
            }
            // set_mario_npc_dialog(MARIO_DIALOG_STOP);
        }
        else
        {
            o->oAglabLakituFriend->oAction = 0;
        }
    }
    else if (LA_S_BETA == o->oAction)
    {
        gMarioSounds = 1;
        if (0 == o->oTimer)
            seq_player_play_sequence(0, SEQ_LEVEL_WATER, 0);

        gCamera->cutscene = 0;
        if (0 == o->oSubAction)
        {
            if (gDialogID == DIALOG_NONE)
            {
                o->oSubAction++;
            }
        }
        else if (1 == o->oSubAction)
        {
            set_fb_effect_col(255, 255, 255, 100);
            set_mario_npc_dialog(MARIO_DIALOG_LOOK_UP);
            create_dialog_box(71);
            o->oSubAction = 2;

            struct Object* obj = spawn_object(o, MODEL_THWOMP, bhvThwomp);
            obj->oPosX = gMarioStates->pos[0];
            obj->oPosY = gMarioStates->pos[1];
            obj->oPosZ = gMarioStates->pos[2];
            obj->oFaceAngleYaw = 0x8000;
            obj->oFaceAnglePitch = 0;
            obj->oFaceAngleRoll = 0;
        }
        else if (2 == o->oSubAction)
        {
            if (gDialogID == DIALOG_NONE)
            {
                gMarioStates->health = 1;
                gWantCustomDeath = 1;
                set_mario_npc_dialog(MARIO_DIALOG_STOP);
                o->oSubAction = 3;
            }
        }
    }
    else if (LA_S_JAMS == o->oAction)
    {
        if (0 == o->oTimer)
            seq_player_play_sequence(0, 0x2d, 0);

        if (0 == o->oTimer)
        {
            // need to set save random flag so game dont complain
            save_file_set_flags(SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR);
            save_file_do_save(gCurrSaveFileNum - 1);     
        }

        gCamera->cutscene = 0;
        if (gDialogID == DIALOG_NONE)
        {
            o->oAction = LA_ENDING_DUALITY - 1;
            show_cs_time_advance_jams(0);
        }
    }
    else if (LA_WHAT == o->oAction)
    {
        if (0 == o->oTimer)
            seq_player_play_sequence(0, 0, 0);

        if (0 == o->oTimer)
        {
            randomize_mao();
        }

        gCamera->cutscene = 0;
        if (gDialogID == DIALOG_NONE)
        {
            gMarioStates->health = 1;
            gWantCustomDeath = 0;
            set_mario_npc_dialog(MARIO_DIALOG_STOP);
        }
    }
    else if (LA_ENDING_DUALITY == o->oAction)
    {
        if (0 == o->oTimer) gCutsceneTimer = 0;
        show_cs_time_advance_jams(CUTSCENE_AGLAB_WINDOW_SHOWCASE);
    }
    else if (LA_ENDING_EVOLUTION == o->oAction)
    {
        if (0 == o->oTimer) gCutsceneTimer = 0;
        show_cs_time_advance_jams(CUTSCENE_AGLAB_TOWERS_SHOWCASE);
    }
    else if (LA_ENDING_DISASTER == o->oAction)
    {
        if (0 == o->oTimer) gCutsceneTimer = 0;
        show_cs_time_advance_jams(CUTSCENE_AGLAB_BRIDGE_SHOWCASE);
    }
    else if (LA_ENDING_SHELL == o->oAction)
    {
        if (0 == o->oTimer) gCutsceneTimer = 0;
        show_cs_time_advance_jams(CUTSCENE_AGLAB_WATER_SHOWCASE);
    }
    else if (LA_ENDING_ONE == o->oAction)
    {
        if (0 == o->oTimer) gCutsceneTimer = 0;
        show_cs_time_advance_jams(CUTSCENE_AGLAB_MAIN_SHOWCASE);
    }
    else if (LA_ENDING_JAMIFY == o->oAction)
    {
        gCamera->cutscene = 0;
        if (0 == o->oTimer)
        {
            struct Object* jam = spawn_object(o, MODEL_CASTLE_GROUNDS_JAM, bhvJam);
            jam->oPosX = gMarioStates->pos[0];
            jam->oPosY = gMarioStates->pos[1] + 80.f;
            jam->oPosZ = gMarioStates->pos[2];
            jam->oFaceAngleYaw = 0;
            jam->oFaceAnglePitch = 0;
            jam->oFaceAngleRoll = 0;
        }
        gMarioObject->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;

        if (gDialogID == DIALOG_NONE)
        {
            gMarioStates->usedObj = o;
            SET_BPARAM2(o->oBehParams, 0xf0);
            level_trigger_warp(gMarioStates, WARP_OP_TELEPORT);
        }
    }
}

void bhv_toad_runner_init(void)
{
    o->oVelZ = -9.f;
    o->oHomeZ = o->oPosZ;
}

static f32 anim_fixup(int fr)
{
    f32 d = 0;
    if (fr < 30)
    {
        return fr * 7.f;
    }

    d += 30 * 7.f;
    fr -= 30;

    if (fr < 30)
    {
        return d + fr * 12.f;
    }
    
    d += 30 * 12.f;
    fr -= 30;

    return d + fr * 13.f;
}

void bhv_toad_runner_loop(void)
{
    if (cur_obj_check_if_near_animation_end())
    {
        o->header.gfx.animInfo.animFrame = 0;
    }
    // print_text_fmt_int(20, 100, "%d", o->header.gfx.animInfo.animFrame);

    o->oOpacity = 255;

    o->oVelZ += random_float() / 100.f;
    o->oHomeZ += o->oVelZ;
    // from 7.f at 30 till 12.5f at 60
    o->oPosZ = o->oHomeZ + anim_fixup(o->header.gfx.animInfo.animFrame);
    struct Surface* floor;
    o->oPosY = find_floor(o->oPosX, o->oPosY + 100.f, o->oHomeZ, &floor);

    if (gBurnToads && o->oHomeZ > -300.f)
    {
        o->activeFlags = 0;
        struct Object* flame = spawn_object(o, MODEL_RED_FLAME, bhvFlame);
        flame->oPosX = o->oPosX;
        flame->oPosY = o->oPosY;
        flame->oPosZ = o->oHomeZ;
    }
}

void bhv_aglab_peach_init()
{
}

void bhv_aglab_peach_loop()
{
    cur_obj_init_animation_with_sound(PEACH_ANIM_KISS);
    o->oOpacity = 0xff;
    // -
}

void bhv_aglab_koopa_init(void)
{
    obj_scale(o, 1.5f);
    o->parentObj = spawn_object(o, MODEL_KOOPA_SHELL, bhvKoopaShell);
    o->oHomeX = o->oPosX;
    o->oHomeY = o->oPosY;
    o->oHomeZ = o->oPosZ;
}

void bhv_aglab_koopa_loop(void)
{
    o->oAglabKoopaAngle += 0x369;
    o->oPosX = o->oHomeX + 300.f * sins(o->oAglabKoopaAngle);
    o->oPosZ = o->oHomeZ + 300.f * coss(o->oAglabKoopaAngle);

    if (100 < o->oTimer && o->oTimer <= 130)
    {
        s32 dt = 130 - o->oTimer;
        f32 y = (30*30 - dt*dt) / 2.f;
        o->oPosY = o->oHomeY + y;
        o->oFaceAngleYaw += 0x1900;
        if (130 == o->oTimer)
        {
            spawn_mist_particles();
        }
    }
    else if (130 < o->oTimer && o->oTimer <= 140)
    {
        s32 dt = 140 - o->oTimer;
        f32 y = dt * 45.f;
        o->oPosY = o->oHomeY + y;
        o->oFaceAngleYaw += 0x2900;
    }
    else
    {
        o->oFaceAngleYaw = 0x4000 + o->oAglabKoopaAngle;
    }

    if (o->oTimer > 140)
    {
        o->oTimer = 0;
    }

    o->parentObj->oFaceAngleYaw = o->oFaceAngleYaw;
    o->parentObj->oPosX = o->oPosX;
    o->parentObj->oPosY = o->oPosY - 40.f;
    o->parentObj->oPosZ = o->oPosZ;

    if (0 == (o->oTimer % 10))
    {
        spawn_object(o, MODEL_WAVE_TRAIL, bhvObjectWaveTrail);
        struct Object *drop = spawn_object_with_scale(o, MODEL_WHITE_PARTICLE_SMALL, bhvWaterDroplet, 1.5f);
        drop->oVelY = random_float() * 30.0f;
        obj_translate_xz_random(drop, 110.0f);
    }
}

void bhv_lava_bg_init()
{
    load_segment_decompress_skybox(0xA,_bitfs_skybox_mio0SegmentRomStart, _bitfs_skybox_mio0SegmentRomEnd);
    obj_scale(o, 0.5f);
}

void bhv_lava_bg_loop()
{
    gMarioSounds = 2;
    if (1 == o->oAction)
    {
        // Spawn flames in the middle of the animation
        s32 animFrame = o->header.gfx.animInfo.animFrame;
        if (animFrame > 24 && animFrame < 36) {
            cur_obj_play_sound_1(SOUND_AIR_BOWSER_SPIT_FIRE);
            if (animFrame == 35) { // Spawns Blue flames at this frame
                spawn_object_relative(1, 0, 200, 50, o, MODEL_RED_FLAME, bhvBlueBowserFlame);
            } else { // Spawns Red flames
                spawn_object_relative(0, 0, 200, 50, o, MODEL_RED_FLAME, bhvBlueBowserFlame);
            }
        }
        // Return to default act once the animation is over
        if (cur_obj_check_if_near_animation_end()) {
            o->oAction = 0;
        }
    }
    else
    {
        cur_obj_init_animation_with_sound(BOWSER_ANIM_RUN);
        if (gPlayer1Controller->buttonPressed & B_BUTTON && gPrevFrameObjectCount < (OBJECT_POOL_CAPACITY - 90))
        {
            o->oAction = 1;
            cur_obj_init_animation_with_sound(BOWSER_ANIM_BREATH_UP);
        }
    }

    o->oPosX = gMarioStates->pos[0];
    o->oPosY = gMarioStates->pos[1];
    o->oPosZ = gMarioStates->pos[2];
    o->oFaceAnglePitch = gMarioStates->faceAngle[0];
    o->oFaceAngleYaw = gMarioStates->faceAngle[1];
    o->oFaceAngleRoll = gMarioStates->faceAngle[2];
    gMarioObject->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;
    o->oOpacity = 255;
}

void bhv_sparkler_loop()
{
    if (0 == (o->oTimer % 16))
    {
        spawn_object(o, MODEL_SPARKLES, bhvCoinSparklesSpawner);
    }
}

extern void spawn_mist_particles_variable_ranged(s32 count, s32 offsetY, f32 size, f32 range);

void bhv_startup_loop(void)
{
    if (o->oTimer == 20)
    {
        play_sound(SOUND_OBJ_THWOMP, gGlobalSoundSource);
        for (int i = 0; i <= 5; i++)
        {
            f32 pos = -750.f + i * 300.f;
            o->oPosX = pos;
            spawn_mist_particles_variable_ranged(30, 0, 46.0f, 100.f);
        }
    }
    if (o->oTimer == 40)
    {
        play_sound(SOUND_OBJ_THWOMP, gGlobalSoundSource);
        o->oPosX = 1126.f;
        spawn_mist_particles_variable_ranged(30, 0, 46.0f, 100.f);
        
        o->oPosX = -1126.f;
        spawn_mist_particles_variable_ranged(30, 0, 46.0f, 100.f);
    }
    if (o->oTimer == 60)
    {
        play_sound(SOUND_GENERAL_SPLATTERING, gGlobalSoundSource);
        o->oPosX = -600.f;
        o->oPosY = 200.f;
        spawn_mist_particles_variable_ranged(30, 0, 46.0f, 100.f);
        
        o->oPosX = 0.f;
        o->oPosY = 200.f;
        spawn_mist_particles_variable_ranged(30, 0, 46.0f, 100.f);

        o->oPosX = 0.f;
        o->oPosY = -300.f;
        spawn_mist_particles_variable_ranged(30, 0, 46.0f, 100.f);

        o->oPosX = 600.f;
        o->oPosY = 200.f;
        spawn_mist_particles_variable_ranged(30, 0, 46.0f, 100.f);
    }
}
