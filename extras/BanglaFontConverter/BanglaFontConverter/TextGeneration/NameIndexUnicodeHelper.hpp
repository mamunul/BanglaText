/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#ifndef NameIndexUnicodeHelper_hpp
#define NameIndexUnicodeHelper_hpp

#include <map>
#include <map>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>

#include "BanglaTextCommon.hpp"
#include "common.hpp"


using namespace std;

class NameIndexUnicodeHelper:public BTAbstractUtility {
private:
    map < string, vector < OutGlyph >> allConjuncts;
    map < uint16_t, uint16_t > unicodeIndexMap;

    vector < int > glyphIndexToUnicodeIndexArray(int glyphIndex);
public:
    NameIndexUnicodeHelper(
        map < string, vector < OutGlyph >> allConjuncts,
        map < uint16_t, uint16_t > unicodeIndexMap
        );
    vector < int > glyphIndexToUnicodeCharArray(int glyphIndex);

    int16_t getGlyphIndexOfUnicode(int16_t code) override;
    int16_t getGlyphUnicodeOfIndex(int16_t index) override;
    int16_t findConjunct(string key, string tag) override;
};

#endif /* NameIndexUnicodeHelper_hpp */
