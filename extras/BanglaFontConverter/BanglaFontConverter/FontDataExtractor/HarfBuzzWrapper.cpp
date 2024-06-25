/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#include "HarfBuzzWrapper.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <cairo-ft.h>
#include <hb-ft.h>
#include <hb-ot.h>
#include <hb.h>
#include <iostream>
#pragma clang diagnostic pop

HarfBuzzWrapper::HarfBuzzWrapper(FontInfo fontInfo, int dpi) {
    this->fontPath = fontInfo.filepathname;
    this->fontName = fontInfo.fontName;
    this->fontSize = fontInfo.fontSize;
    this->dpi = dpi;
}

vector<HBGlyphProperty> get_glyph_properties_from(hb_buffer_t *hb_buffer, hb_font_t *hb_font) {
    /* Get glyph information and positions out of the buffer. */
    unsigned int len = hb_buffer_get_length(hb_buffer);
    hb_glyph_info_t *info = hb_buffer_get_glyph_infos(hb_buffer, NULL);
    hb_glyph_position_t *pos = hb_buffer_get_glyph_positions(hb_buffer, NULL);

    vector<HBGlyphProperty> glyphs;

    for (unsigned int i = 0; i < len; i++) {
        hb_codepoint_t gid = info[i].codepoint;
        float x_advance = pos[i].x_advance / 64.;
        float y_advance = pos[i].y_advance / 64.;
        float x_offset = pos[i].x_offset / 64.;
        float y_offset = pos[i].y_offset / 64.;

        char glyphname[32];
        hb_font_get_glyph_name(hb_font, gid, glyphname, sizeof(glyphname));


        hb_glyph_extents_t hbextents;
        hb_font_get_glyph_extents(hb_font, info[i].codepoint, &hbextents);

        HBGlyphProperty glyph;
        glyph.x_advance = x_advance;
        glyph.y_advance = y_advance;
        glyph.x_offset = x_offset;
        glyph.y_offset = y_offset;
        glyph.glyphname = glyphname;
        glyph.gid = gid;
        glyph.width = hbextents.width / 64.;
        glyph.height = hbextents.height / 64.;

        glyphs.push_back(glyph);
    }

    return glyphs;
}

vector<HBGlyphProperty> HarfBuzzWrapper::get_glyph_properties_open(
    string text
    ) {
    /* Create a font. */
    hb_blob_t *blob = hb_blob_create_from_file(fontPath.c_str());
    hb_face_t *hb_face = hb_face_create(blob, 0);

    hb_blob_destroy(blob); /* face will keep a reference to it */
    hb_font_t *hb_font = hb_font_create(hb_face);

    hb_ot_font_set_funcs(hb_font);
    hb_font_set_ptem(hb_font, dpi);
    hb_font_set_scale(hb_font, fontSize * 64, fontSize * 64);


    /* Create hb-buffer and populate. */
    hb_buffer_t *hb_buffer;
    hb_buffer = hb_buffer_create();
    hb_buffer_add_utf8(hb_buffer, text.c_str(), -1, 0, -1);
    hb_buffer_guess_segment_properties(hb_buffer);

    /* Shape it! */
    hb_shape(hb_font, hb_buffer, NULL, 0);

    return get_glyph_properties_from(hb_buffer, hb_font);
}

vector<HBGlyphProperty> HarfBuzzWrapper::get_glyph_properties_open(
    vector<int> unicodeArray
    ) {
    //    vector<int> v = {2453,2497};
    //    vector<int> v = {2470,2509,2477,2497};
    string text = unicode_to_string(unicodeArray);

    return get_glyph_properties_open(text);
}

vector<HBGlyphProperty> HarfBuzzWrapper::get_glyph_properties(
    string text
    ) {
    FT_Library ft_library;
    FT_Face ft_face;
    FT_Error ft_error;

    if ((ft_error = FT_Init_FreeType(&ft_library))) {
        abort();
    }

    if ((ft_error = FT_New_Face(ft_library, fontPath.c_str(), 0, &ft_face))) {
        abort();
    }

    if ((ft_error = FT_Set_Char_Size(ft_face, fontSize * 64, fontSize * 64, dpi, 0))) {
        abort();
    }

    /* Create hb-ft font. */
    hb_font_t *hb_font;
    hb_font = hb_ft_font_create(ft_face, NULL);

    /* Create hb-buffer and populate. */
    hb_buffer_t *hb_buffer;
    hb_buffer = hb_buffer_create();
    hb_buffer_add_utf8(hb_buffer, text.c_str(), -1, 0, -1);
    hb_buffer_guess_segment_properties(hb_buffer);

    /* Shape it! */
    hb_shape(hb_font, hb_buffer, NULL, 0);

    return get_glyph_properties_from(hb_buffer, hb_font);
}

vector<HBGlyphProperty> HarfBuzzWrapper::get_glyph_properties(
    vector<int> unicodeArray
    ) {
    //    vector<int> v = {2453,2497};
    //    vector<int> v = {2470,2509,2477,2497};
    string text = unicode_to_string(unicodeArray);

    return get_glyph_properties(text);
}
