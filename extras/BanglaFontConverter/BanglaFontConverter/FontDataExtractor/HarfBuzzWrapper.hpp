/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#ifndef HarfBuzzWrapper_hpp
#define HarfBuzzWrapper_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include "common.hpp"

using namespace std;

struct HBGlyphProperty {
    float x_advance;
    float y_advance;
    float x_offset;
    float y_offset;
    string glyphname;
    int gid;
    float width;
    float height;
};

class HarfBuzzWrapper {
private:
    string fontPath;
    string fontName;
    int fontSize;
    int dpi;

public:
    HarfBuzzWrapper(FontInfo fontInfo, int dpi);

    vector < HBGlyphProperty > get_glyph_properties(vector < int > unicodeArray);
    vector < HBGlyphProperty > get_glyph_properties_open(vector < int > unicodeArray);
    vector < HBGlyphProperty > get_glyph_properties(string text);
    vector < HBGlyphProperty > get_glyph_properties_open(string text);

//    vector<HBGlyphProperty> get_glyph_properties(hb_buffer_t *hb_buffer, hb_font_t *hb_font);
};

#endif /* HarfBuzzWrapper_hpp */
