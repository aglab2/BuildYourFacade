#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/level_update.h"

#include "levels/scripts.h"

#include "actors/common1.h"

/* Fast64 begin persistent block [includes] */
/* Fast64 end persistent block [includes] */

#include "make_const_nonconst.h"
#include "levels/castle_grounds/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_castle_grounds_entry[] = {
	INIT_LEVEL(),
	LOAD_YAY0(0x07, _castle_grounds_segment_7SegmentRomStart, _castle_grounds_segment_7SegmentRomEnd), 
	LOAD_YAY0(0x0A, _water_skybox_yay0SegmentRomStart, _water_skybox_yay0SegmentRomEnd), 
	LOAD_YAY0_TEXTURE(0x09, _outside_yay0SegmentRomStart, _outside_yay0SegmentRomEnd), 
	LOAD_YAY0(0x05, _group10_yay0SegmentRomStart, _group10_yay0SegmentRomEnd), 
	LOAD_RAW(0x0C, _group10_geoSegmentRomStart, _group10_geoSegmentRomEnd), 
	LOAD_YAY0(0x06, _group15_yay0SegmentRomStart, _group15_yay0SegmentRomEnd), 
	LOAD_RAW(0x0D, _group15_geoSegmentRomStart, _group15_geoSegmentRomEnd), 
	LOAD_YAY0(0x08, _common0_yay0SegmentRomStart, _common0_yay0SegmentRomEnd), 
	LOAD_RAW(0x0F, _common0_geoSegmentRomStart, _common0_geoSegmentRomEnd), 
	LOAD_MIO0(0x7, _castle_grounds_segment_7SegmentRomStart, _castle_grounds_segment_7SegmentRomEnd), 
	LOAD_MIO0(0xa, _water_skybox_mio0SegmentRomStart, _water_skybox_mio0SegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_1), 
	JUMP_LINK(script_func_global_11), 
	JUMP_LINK(script_func_global_16), 
	LOAD_MODEL_FROM_GEO(MODEL_LEVEL_GEOMETRY_03, castle_grounds_geo_0006F4), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_BUBBLY_TREE, bubbly_tree_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_WARP_PIPE, warp_pipe_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_CASTLE_DOOR, castle_door_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_METAL_DOOR, metal_door_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_VCUTM_GRILL, btowers_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_FLAG, etowers_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_CANNON_GRILL, vtowers_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_ONE_TOWER, one_tower_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_DISASTER_BRIDGE, disaster_bridge_geo), 
	LOAD_MODEL_FROM_GEO(MODEL_CASTLE_GROUNDS_JAM, jam_geo),

	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, castle_grounds_area_1),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xf0, LEVEL_ENDING, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xf1, LEVEL_CASTLE_COURTYARD, 0x01, 0x0a, WARP_NO_CHECKPOINT),
		WARP_NODE(0xf3, LEVEL_CASTLE_GROUNDS, 0x01, 0xf1, WARP_NO_CHECKPOINT),
		WARP_NODE(0xb, LEVEL_CASTLE_COURTYARD, 0x02, 0x0a, WARP_NO_CHECKPOINT),
		WARP_NODE(0xc, LEVEL_CASTLE_COURTYARD, 0x03, 0x0a, WARP_NO_CHECKPOINT),
		WARP_NODE(0xd, LEVEL_CASTLE_COURTYARD, 0x04, 0x0a, WARP_NO_CHECKPOINT),
		WARP_NODE(0xe, LEVEL_CASTLE_COURTYARD, 0x05, 0x0a, WARP_NO_CHECKPOINT),
		MARIO_POS(0x01, 0, 1275, -185, -4715),
		OBJECT(MODEL_LAKITU, -183, 669, -493, 0, 0, 0, 0x00000000, bhvAglabLakitu),
		OBJECT(MODEL_NONE, 1290, 670, -4715, 0, 0, 0, (0xa << 16), bhvInstantActiveWarp),
		OBJECT(MODEL_NONE, 1270, 654, -4707, 0, 0, 0, (0xf1 << 16), bhvAirborneDeathWarp),
		OBJECT(MODEL_NONE, 1538, -185, -3925, 0, 90, 0, (4 << 16), bhvCoinFormation),
		OBJECT(MODEL_NONE, 3824, -185, -2738, 0, 0, 0, (4 << 16), bhvCoinFormation),
		OBJECT(MODEL_NONE, 3131, -85, -486, 0, -90, 0, (4 << 16), bhvCoinFormation),
		TERRAIN(castle_grounds_area_1_collision),
		MACRO_OBJECTS(castle_grounds_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, 0x00),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),

	FREE_LEVEL_POOL(),
	MARIO_POS(0x01, 0, 1275, -185, -4715),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};
