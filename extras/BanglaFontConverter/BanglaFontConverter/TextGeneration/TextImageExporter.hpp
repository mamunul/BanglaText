/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#ifndef TextImageExporter_hpp
#define TextImageExporter_hpp

#include <map>
#include <stdio.h>
#include <string>

#include "CairoWrapper.hpp"
#include "FontDataExtractor.hpp"
#include "NameIndexUnicodeHelper.hpp"

using namespace std;

class TextImageExporter {
private:
    CairoWrapper *crWrapper;
    NameIndexUnicodeHelper *helper;
    map < string, vector < OutGlyph >> allConjuncts;
    map < string, BTOffset > allMarkPos;

public:
    TextImageExporter(
        CairoWrapper * crWrapper,
        NameIndexUnicodeHelper * helper,
        map < string, vector < OutGlyph >> allConjuncts,
        map < string, BTOffset > allMarkPos
        );
    void renderAllConjuncts();
    void renderConjuct(int glyphIndex);

    void renderMarkBase(int markIndex, int baseIndex);
    void renderAllMarkBase();
    void exportTextImage(string text, string outputFile);
};

#endif /* TextImageExporter_hpp */
