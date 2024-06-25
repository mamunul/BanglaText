/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#ifndef FeatureProcessor_hpp
#define FeatureProcessor_hpp

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "FreetypeWrapper.hpp"
#include "BanglaTextCommon.hpp"

using namespace std;

class FontDataExtractor {
public:
    FontDataExtractor(FontInfo fontInfo);

    void logGSUBData();
    void logGPOSData();

    map < string, vector < OutGlyph >> getAllConjuncts();
    map < string, BTOffset > getAllMarkBaseOffset();
    map < uint16_t, uint16_t > getUnicodeIndexMap();

    void test();


private:


    map < string, vector < OutGlyph >> allConjuncts2;
    map < string, BTOffset > allMarkBaseOffset2;
    map < string, int > nameIndexMap;
    map < uint16_t, uint16_t > unicodeIndexMap;

    map < string, map < string, vector < pair < string, string >> >> processGSUBJsonFile(string filepath);
    map < string, map < string, pair < int, int >> > processGPOSJsonFile(string filepath);
    map < string, int > processNameMapJsonFile(string filepath);
    map < uint16_t, uint16_t > processUnicodeIndexMapJsonFile(string unicodeMapFile);
};

#endif /* FeatureProcessor_hpp */
