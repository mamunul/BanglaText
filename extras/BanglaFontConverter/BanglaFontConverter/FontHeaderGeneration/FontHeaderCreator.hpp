/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#ifndef FontHeaderCreator_hpp
#define FontHeaderCreator_hpp

#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <vector>

#include "FontDataExtractor.hpp"
#include "FreetypeWrapper.hpp"
#include "HarfBuzzWrapper.hpp"
#include "NameIndexUnicodeHelper.hpp"

using namespace std;

class FontHeaderCreator {
    FreetypeWrapper *freetype;
    FontDataExtractor *extractor;
    HarfBuzzWrapper *hb;
    NameIndexUnicodeHelper *helper;
    string fontName;

    pair < string, int > getFontHeaderContent();
public:
    FontHeaderCreator(
        string fontName,
        FreetypeWrapper * freetype,
        FontDataExtractor * extractor,
        HarfBuzzWrapper * hb,
        NameIndexUnicodeHelper * helper
        );
    void generateHeaderFile(string filePath);
    void consoleLogHeaderFile();
};

#endif /* FontHeaderCreator_hpp */
