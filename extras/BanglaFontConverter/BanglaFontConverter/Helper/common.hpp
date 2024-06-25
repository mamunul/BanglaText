/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#ifndef common_h
#define common_h

#include <sstream>
#include <string>
#include <vector>
#include <map>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <unicode/usearch.h>
#pragma clang diagnostic pop

using namespace std;

extern string all_conjuct_words;
extern string all_independent_vowels;
extern string two_part_dependent_vowels;
extern string all_dependent_vowels;
extern string all_consontants;
extern string all_special_character;

struct OutGlyph
{
    string tag;
    int index;
};

struct FontInfo {
    int fontSize;
    string filepathname;
    string fontName;
    string gsubFile;
    string gposFile;
    string nameMapFile;
    string unicodeMapFile;
};

string unicode_to_string(vector<int> unicodeArray);
#endif /* common_h */
