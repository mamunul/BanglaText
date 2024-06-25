/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#include "NameIndexUnicodeHelper.hpp"

vector<int>  implement_blwf(vector<int> unicodeArray) {
    for (int i = 0; i < unicodeArray.size() - 1; i++) {
        if (
            unicodeArray[i + 1] == Virama &&
            (unicodeArray[i] == RaMiddle || unicodeArray[i] == Ba
             || unicodeArray[i] == Ra)
            ) {
            swap(unicodeArray[i + 1], unicodeArray[i]);
        }
    }

    return unicodeArray;
}

int16_t NameIndexUnicodeHelper::findConjunct(string key, string tag){
    
    if (allConjuncts.find(key) != allConjuncts.end()) {
        if (allConjuncts[key].size() == 1) {
            return allConjuncts[key][0].index;
        }

        for (auto t:allConjuncts[key]) {
            if (t.tag == tag) {
                return t.index;
            }
        }
    }
 
    return -1;
}

int16_t NameIndexUnicodeHelper::getGlyphUnicodeOfIndex(int16_t index) {
    for (auto it = unicodeIndexMap.begin(); it != unicodeIndexMap.end(); ++it) {
        if (it->second == index) {
            return it->first;
        }
    }

    return -1;
}

int16_t NameIndexUnicodeHelper::getGlyphIndexOfUnicode(int16_t code) {
    return unicodeIndexMap[code];
}

NameIndexUnicodeHelper::NameIndexUnicodeHelper(
    map < string, vector < OutGlyph > > allConjuncts,
    map < uint16_t, uint16_t > unicodeIndexMap
    ) {
    this->allConjuncts = allConjuncts;
    this->unicodeIndexMap = unicodeIndexMap;
}

vector<int> NameIndexUnicodeHelper::glyphIndexToUnicodeCharArray(int glyphIndex) {
    auto indices = glyphIndexToUnicodeIndexArray(glyphIndex);

    vector<int> unicodeArray;

    for (int index:indices) {
        int unicode = getGlyphUnicodeOfIndex(index);
        unicodeArray.push_back(unicode);
    }

    return unicodeArray;
}

vector<int> NameIndexUnicodeHelper::glyphIndexToUnicodeIndexArray(int glyphIndex) {
    if (getGlyphUnicodeOfIndex(glyphIndex) > 0) {
        return vector<int>{ glyphIndex };
    }

    for (auto it : allConjuncts) {
        auto vit = find_if(
            it.second.begin(), it.second.end(),
            [glyphIndex](const OutGlyph &p)
            {
                return p.index == glyphIndex;
            });

        if (vit != it.second.end()) {
            vector<string> indices = split(it.first, ',');
            vector<int> all;

            for (int i = 0; i < indices.size(); i++) {
                vector<int> one = glyphIndexToUnicodeIndexArray(stoi(indices[i]));
                all.insert(all.end(), one.begin(), one.end());
            }

            return all;
        }
    }

    return vector<int>{};
}
