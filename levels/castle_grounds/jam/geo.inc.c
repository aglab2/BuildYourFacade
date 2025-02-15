#include "src/game/envfx_snow.h"

const GeoLayout jam_geo[] = {
   GEO_CULLING_RADIUS(0x7fff),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(1, jam_Jam_mesh_layer_1),
		GEO_DISPLAY_LIST(5, jam_Jam_mesh_layer_5),
		GEO_DISPLAY_LIST(1, jam_material_revert_render_settings),
		GEO_DISPLAY_LIST(5, jam_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};