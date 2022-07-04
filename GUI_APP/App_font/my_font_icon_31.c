/*******************************************************************************
 * Size: 30 px
 * Bpp: 4
 * Opts: --no-compress --format lvgl --font C:\Users\asus\Desktop\iconfont.ttf -o C:\Users\asus\Desktop\my_font_icon_31.c --bpp 4 --size 30 -r 0xe6de
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef MY_FONT_ICON_31
#define MY_FONT_ICON_31 1
#endif

#if MY_FONT_ICON_31

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+E6DE "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x9, 0xff, 0xc4, 0x0, 0x0, 0x0, 0xc, 0xfd,
    0xcf, 0xf4, 0x0, 0x0, 0x6, 0xfa, 0x0, 0x3f,
    0xd0, 0x0, 0x0, 0x9f, 0x20, 0x0, 0xbf, 0x0,
    0x0, 0xa, 0xf1, 0x0, 0xa, 0xf1, 0x0, 0x0,
    0xaf, 0x10, 0x0, 0xaf, 0x10, 0x0, 0xa, 0xf1,
    0x0, 0xa, 0xf1, 0x0, 0x0, 0xaf, 0x10, 0x0,
    0xaf, 0x10, 0x0, 0xa, 0xf1, 0x12, 0xa, 0xf1,
    0x0, 0x0, 0xaf, 0x1e, 0xf3, 0xaf, 0x10, 0x0,
    0xa, 0xf2, 0xff, 0x6a, 0xf1, 0x0, 0x0, 0xaf,
    0x2f, 0xf6, 0xaf, 0x10, 0x0, 0xa, 0xf2, 0xff,
    0x6a, 0xf1, 0x0, 0x0, 0xaf, 0x2f, 0xf6, 0xaf,
    0x10, 0x0, 0xa, 0xf2, 0xff, 0x6a, 0xf1, 0x0,
    0x0, 0xbf, 0x2f, 0xf6, 0xaf, 0x20, 0x0, 0x7f,
    0xe1, 0xff, 0x67, 0xfd, 0x0, 0x2f, 0xe3, 0xbf,
    0xfe, 0x49, 0xf8, 0x8, 0xf5, 0xdf, 0xff, 0xff,
    0x3e, 0xe0, 0xcf, 0x4f, 0xff, 0xff, 0xfa, 0x9f,
    0x3d, 0xe6, 0xff, 0xff, 0xff, 0xc7, 0xf4, 0xcf,
    0x5f, 0xff, 0xff, 0xf9, 0x9f, 0x28, 0xf6, 0xdf,
    0xff, 0xff, 0x4f, 0xe0, 0x1f, 0xf5, 0xcf, 0xfe,
    0x6b, 0xf6, 0x0, 0x5f, 0xf8, 0x66, 0x6d, 0xfa,
    0x0, 0x0, 0x5e, 0xff, 0xff, 0xf8, 0x0, 0x0,
    0x0, 0x5, 0x99, 0x72, 0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 480, .box_w = 13, .box_h = 28, .ofs_x = 9, .ofs_y = -3}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 59102, .range_length = 1, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t my_font_icon_31 = {
#else
lv_font_t my_font_icon_31 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 28,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0)
    .underline_position = 0,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if MY_FONT_ICON_31*/

