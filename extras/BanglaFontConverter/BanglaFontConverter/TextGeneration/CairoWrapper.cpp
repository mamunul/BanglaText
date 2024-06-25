/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#include "CairoWrapper.hpp"

CairoWrapper::CairoWrapper(
    HarfBuzzWrapper *hb,
    FontInfo        fontInfo,
    int             dpi
    ) : hb(hb) {
    this->fontPath = fontInfo.filepathname;
    this->fontName = fontInfo.fontName;
    this->fontSize = fontInfo.fontSize;
    this->dpi = dpi;
}

int CairoWrapper::hbfreeExportImage(
    vector<int> unicodeArray,
    string      outfile
    ) {
    string text = unicode_to_string(unicodeArray);

    cout << text << endl;

    hbfreeExportImage(text, outfile);

    return 0;
}

int CairoWrapper::hbfreeExportImage(
    string text,
    string outfile
    ) {
    string outputFile = "build/" + outfile;

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

//    bool is_horizontal = HB_DIRECTION_IS_HORIZONTAL(hb_buffer_get_direction(hb_buffer));

    vector<HBGlyphProperty> glyphsInfo = hb->get_glyph_properties(text);

    export_text_image(outputFile,  ft_face, glyphsInfo);

    FT_Done_Face(ft_face);
    FT_Done_FreeType(ft_library);

    return 0;
}

void CairoWrapper::export_text_image(
    string                  outputFile,
    FT_Face                 ft_face,
    vector<HBGlyphProperty> glyphsInfo
    ) {
    double margin = (fontSize * .5);
    /* Draw, using cairo. */
    double width = 2 * margin;
    double height = 2 * margin;

    for (unsigned int i = 0; i < glyphsInfo.size(); i++) {
        width += glyphsInfo[i].x_advance;
        height -= glyphsInfo[i].y_advance;
    }

    height += fontSize;

    /* Set up cairo surface. */
    cairo_surface_t *cairo_surface;
    cairo_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                               ceil(width),
                                               ceil(height));
    cairo_t *cr;
    cr = cairo_create(cairo_surface);
    cairo_set_source_rgba(cr, 1., 1., 1., 1.);
    cairo_paint(cr);
    cairo_set_source_rgba(cr, 0., 0., 0., 1.);
    cairo_translate(cr, margin, margin);

    /* Set up cairo font face. */
    cairo_font_face_t *cairo_face;
    cairo_face = cairo_ft_font_face_create_for_ft_face(ft_face, 0);
    cairo_set_font_face(cr, cairo_face);
    cairo_set_font_size(cr, fontSize);

    /* Set up baseline. */
//    if (is_horizontal) {
    cairo_font_extents_t font_extents;
    cairo_font_extents(cr, &font_extents);
    double baseline = (fontSize - font_extents.height) * .5 + font_extents.ascent;
    cairo_translate(cr, 0, baseline);
//    } else {
//        cairo_translate(cr, fontSize * .5, 0);
//    }

    cairo_glyph_t *cairo_glyphs = cairo_glyph_allocate((int)glyphsInfo.size());
    double current_x = 0;
    double current_y = 0;

    for (unsigned int i = 0; i < glyphsInfo.size(); i++) {
        cairo_glyphs[i].index = glyphsInfo[i].gid;
        cairo_glyphs[i].x = current_x + glyphsInfo[i].x_offset;
        cairo_glyphs[i].y = -(current_y + glyphsInfo[i].y_offset);
        current_x += glyphsInfo[i].x_advance;
        current_y += glyphsInfo[i].y_advance;
        cairo_text_extents_t extents;
        cairo_glyph_extents(cr, &cairo_glyphs[i], 1, &extents);
    }

    cairo_show_glyphs(cr, cairo_glyphs, (int)glyphsInfo.size());
    cairo_glyph_free(cairo_glyphs);

    cairo_surface_write_to_png(cairo_surface, outputFile.c_str());

    cairo_font_face_destroy(cairo_face);
    cairo_destroy(cr);
    cairo_surface_destroy(cairo_surface);
}
