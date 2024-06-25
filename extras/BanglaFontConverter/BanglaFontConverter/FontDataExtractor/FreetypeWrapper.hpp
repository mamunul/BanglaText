/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#ifndef font_processor_h
#define font_processor_h

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include <ft2build.h>

#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_MODULE_H
#include FT_TRUETYPE_DRIVER_H

#include FT_CONFIG_OPTIONS_H
#include FT_OPENTYPE_VALIDATE_H

#pragma clang diagnostic pop

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "common.hpp"

using namespace std;

typedef struct Glyph_Info {
    int gid;
    int width;
    int height;
    float x_advance;
    float y_advance;
    int x_offset;
    int y_offset;
    float bearing_x;
    float bearing_y;
    string glyph_name;
    vector < uint8_t > processed_bitmap;
    FT_Bitmap *bitmap;
    FT_Glyph glyph;
}Glyph_Info;

class FreetypeWrapper {
private:
    string fontFilePath;
    int fontSize;
    int dpi;
    FT_Library library;
    FT_Face face;
    int totalGlyphCount;

    void loadFont();
    void loadAllUnicodeIndexMap();

public:
    FreetypeWrapper(FontInfo fontInfo, int dpi);
    ~FreetypeWrapper();

    int getTotalGlyphCount();


    Glyph_Info getGlyph(int glyphIndex);
    vector < Glyph_Info > getGlyph(int startIndex, int endIndex);
    vector < Glyph_Info > getAllGlyph();
    void logGlyph(Glyph_Info info);

};

#endif /* font_processor_h */
