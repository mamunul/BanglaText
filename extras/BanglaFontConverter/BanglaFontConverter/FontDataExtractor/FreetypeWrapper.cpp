/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#include "FreetypeWrapper.hpp"



#include <format>
#include <iomanip>
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include "common.hpp"

// Accumulate bits for output, with periodic hexadecimal byte write
int16_t enbit(uint8_t value) {
    static int16_t sum = 0, bit = 0x80;

    if (value) {
        sum |= bit;      // Set bit if needed
    }

    if (!(bit >>= 1)) {  // Advance to next bit, end of byte reached?
        int16_t ss = sum;
        sum = 0;         // Clear for next byte
        bit = 0x80;      // Reset bit counter
        return ss;
    }

    return -1;
}

vector<uint8_t> process_bitmap_for_glyph(Glyph_Info info) {
    int byte;
    uint8_t bit;   vector<uint8_t> bitmap_data;

    for (int y = 0; y < info.height; y++) {
        for (int x = 0; x < info.width; x++) {
            byte = x / 8;
            bit = 0x80 >> (x & 7);
            int16_t p = enbit(info.bitmap->buffer[y * info.bitmap->pitch + byte] & bit);

            if (p >= 0) {
                bitmap_data.push_back((uint8_t)p);
            }
        }
    }

    // Pad end of char bitmap to next byte boundary if needed
    int n = (info.width * info.height) & 7;

    if (n) {        // Pixel count not an even multiple of 8?
        n = 8 - n;  // # bits to next multiple

        while (n--) {
            int16_t p = enbit(0);

            if (p >= 0) {
                bitmap_data.push_back((uint8_t)p);
            }
        }
    }

    return bitmap_data;
}

FreetypeWrapper::FreetypeWrapper(FontInfo fontInfo, int dpi) {
    this->fontFilePath = fontInfo.filepathname;
    this->fontSize = fontInfo.fontSize;
    this->dpi = dpi;

    loadFont();
}

FreetypeWrapper::~FreetypeWrapper() {
    FT_Done_Library(library);
}


int FreetypeWrapper::getTotalGlyphCount() {
    return totalGlyphCount;
}

Glyph_Info FreetypeWrapper::getGlyph(int glyphIndex) {
    int err;
    FT_Glyph glyph;
    FT_Bitmap *bitmap;
    FT_BitmapGlyphRec *g;

    if ((err =  FT_Load_Glyph(face, glyphIndex, FT_LOAD_TARGET_MONO))) {
        fprintf(stderr, "Error %d loading glyph index '%i'\n", err, glyphIndex);
    }

    if ((err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO))) {
        fprintf(stderr, "Error %d rendering glyph index '%i'\n", err, glyphIndex);
    }

    if ((err = FT_Get_Glyph(face->glyph, &glyph))) {
        fprintf(stderr, "Error %d getting glyph glyph index '%i'\n", err, glyphIndex);
    }

    string glyphName = "";// getGlyphName(glyphIndex);

    bitmap = &face->glyph->bitmap;
    g = (FT_BitmapGlyphRec *)glyph;

    Glyph_Info info;

    info.glyph_name = glyphName;
    info.gid = glyphIndex;
    info.width = bitmap->width;
    info.height = bitmap->rows;
    float xadv = (face->glyph->advance.x / 64.);
    info.x_advance = xadv;
    info.bearing_x = face->glyph->bitmap_left;
    info.bearing_y = face->glyph->bitmap_top;
    xadv = (face->glyph->advance.y / 64.);
    info.y_advance =  xadv;
    info.x_offset = info.bearing_x;
    info.y_offset = info.bearing_y;
    info.bitmap = bitmap;
    info.glyph = glyph;

    vector<uint8_t> processed_bitmap = process_bitmap_for_glyph(info);
    info.processed_bitmap = processed_bitmap;

    FT_Done_Glyph(glyph);

    return info;
}

vector<Glyph_Info> FreetypeWrapper::getGlyph(int startIndex, int endIndex) {
    vector<Glyph_Info> infoList;

    for (int i = startIndex; i <= endIndex; i++) {
        Glyph_Info info = getGlyph(i);
        infoList.push_back(info);
    }

    return infoList;
}

vector<Glyph_Info> FreetypeWrapper::getAllGlyph() {
    vector<Glyph_Info> infoList = getGlyph(0, getTotalGlyphCount()-1);

    return infoList;
}

void FreetypeWrapper::loadFont() {
    int err = 0;

    if ((err = FT_Init_FreeType(&library))) {
        printf("Init_FreeType Error!\n");
    }

    FT_UInt interpreter_version = TT_INTERPRETER_VERSION_35;
    FT_Property_Set(library, "truetype", "interpreter-version", &interpreter_version);

    if ((err = FT_New_Face(library, fontFilePath.c_str(), 0, &face))) {
        fprintf(stderr, "Font load error: %d", err);
        FT_Done_FreeType(library);
    }

    // << 6 because '26dot6' fixed-point format
    FT_Set_Char_Size(face, fontSize << 6, 0, dpi, 0);

    totalGlyphCount = (int)face->num_glyphs;
}

void FreetypeWrapper::logGlyph(Glyph_Info info) {
    printf("glyph index:%i \n", info.gid);
    printf("glyph name:%s, %lu\n", info.glyph_name.c_str(), info.glyph_name.length());
    //    printf("bitmapOffset:%i \n", bitmapOffset);
    printf("width:%i \n", info.width);
    printf("height:%i \n", info.height);
    printf("xAdvance:%f \n", info.x_advance);
    printf("xOffset:%i \n", info.x_offset);
    printf("yOffset:%i \n", info.y_offset);
}
