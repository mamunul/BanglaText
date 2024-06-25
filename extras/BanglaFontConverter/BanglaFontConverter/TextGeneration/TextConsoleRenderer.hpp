/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#ifndef TextRenderer_hpp
#define TextRenderer_hpp

#include <stdio.h>
#include <string>

#include "BTGlyphDecoder.hpp"
#include "FontDataExtractor.hpp"
#include "FreetypeWrapper.hpp"
#include "HarfBuzzWrapper.hpp"
#include "NameIndexUnicodeHelper.hpp"

using namespace std;

class TextConsoleRenderer {
    FreetypeWrapper *freetypepro;
    BTGlyphDecoder *glyphProcessor;
    HarfBuzzWrapper *hbwrapper;
    NameIndexUnicodeHelper *helper;

    void renderGlyphArray(vector < int16_t > unicodeArray);
    bool checkMarkIndex(int16_t index);

public:
    TextConsoleRenderer(
        FreetypeWrapper * freetypepro,
        BTGlyphDecoder * glyphProcessor,
        HarfBuzzWrapper * hbwrapper,
        NameIndexUnicodeHelper * helper
        );
    void logText(string text);
};

#endif /* TextRenderer_hpp */
