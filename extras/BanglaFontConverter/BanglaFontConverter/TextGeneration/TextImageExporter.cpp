/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#include "TextImageExporter.hpp"


TextImageExporter::TextImageExporter(
    CairoWrapper *crWrapper,
    NameIndexUnicodeHelper *helper,
    map < string, vector < OutGlyph > > allConjuncts,
    map < string, BTOffset > allMarkPos
    ) : crWrapper(crWrapper), helper(helper) {
    this->allConjuncts = allConjuncts;
    this->allMarkPos = allMarkPos;
}

void TextImageExporter::exportTextImage(string text, string outputFile) {
    string file = text  + ".png";
    crWrapper->hbfreeExportImage(text, file);
}

void TextImageExporter::renderConjuct(int glyphIndex) {
    vector<int> unicodeIntArray = helper->glyphIndexToUnicodeCharArray(glyphIndex);
    string file = to_string(glyphIndex)  + ".png";
    crWrapper->hbfreeExportImage(unicodeIntArray, file);
}

void TextImageExporter::renderAllConjuncts() {
    for (auto it : allConjuncts) {
        for (auto v : it.second) {
            renderConjuct(v.index);
        }
    }
}

void TextImageExporter::renderMarkBase(int markIndex, int baseIndex) {
    vector<int> unicodeIntArray = helper->glyphIndexToUnicodeCharArray(baseIndex);
    
    int markUnicode = helper->getGlyphUnicodeOfIndex(markIndex);
    unicodeIntArray.push_back(markUnicode);
    string file = to_string(baseIndex) + "_" + to_string(markIndex) + ".png";
    crWrapper->hbfreeExportImage(unicodeIntArray, file);
}

void TextImageExporter::renderAllMarkBase() {
    for (auto it : allMarkPos) {
        auto markbase = split(it.first, ',');
        renderMarkBase(stoi(markbase[0]), stoi(markbase[1]));
    }
}
