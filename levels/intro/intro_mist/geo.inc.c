// 0x16000000
extern const Gfx intro_mist_seg3_dl_03000880[];
const GeoLayout intro_mist_geo[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_ASM(GEO_TRANSPARENCY_MODE_NORMAL, geo_update_layer_transparency),
      GEO_DISPLAY_LIST(LAYER_TRANSPARENT, intro_mist_seg3_dl_03000880),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
