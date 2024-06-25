/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#ifndef CairoWrapper_hpp
#define CairoWrapper_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include "common.hpp"
#include "HarfBuzzWrapper.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <cairo-ft.h>
#include <hb-ft.h>
#include <hb-ot.h>
#include <hb.h>
#include <iostream>
#pragma clang diagnostic pop

using namespace std;

class CairoWrapper {
private:
    HarfBuzzWrapper *hb;
    string fontPath;
    string fontName;
    int fontSize;
    int dpi;

    void export_text_image(
        string                     outputFile,
        FT_Face                    ft_face,
        vector < HBGlyphProperty > glyphsInfo
        );
public:
    CairoWrapper(
        HarfBuzzWrapper * hb,
        FontInfo fontInfo,
        int dpi
        );

    int hbfreeExportImage(vector < int > unicodeArray, string outfile);

    int hbfreeExportImage(string text, string outfile);
};


#endif /* CairoWrapper_hpp */
