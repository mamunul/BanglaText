/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#ifndef GlyphDecoder2_hpp
#define GlyphDecoder2_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include "BanglaTextUtility.hpp"

// using namespace std;

class BTGlyphDecoder {
private:
    BTAbstractUtility *helper;

    std::vector < int16_t > matchIIVowelGlyph(std::vector < int16_t > indexArray);
    std::vector < int16_t > matchRiVowelGlyph(std::vector < int16_t > indexArray);
    std::vector < int16_t > matchUUVowelGlyph(std::vector < int16_t > indexArray);
    std::vector < int16_t > matchUVowelGlyph(std::vector < int16_t > indexArray);
    std::vector < int16_t > matchFota(std::vector < int16_t > indexArray);
    std::vector < int16_t > rearrangeUKar(std::vector < int16_t > indexArray);
    std::vector < int16_t > rearrangeRef(std::vector < int16_t > indexArray);
    std::vector < int16_t > rearrangeKar(std::vector < int16_t > indexArray);
    std::vector < int16_t > matchRef(std::vector < int16_t > indexArray);
    std::vector < int16_t > rearrangeTwoVowel(std::vector < int16_t > indexArray);
    std::vector < int16_t > matchJaFola(std::vector < int16_t > indexArray);
    std::vector < int16_t > matchTrippleConsonantGlyph(std::vector < int16_t > indexArray);
    std::vector < int16_t > matchTwoConsonantGlyph(std::vector < int16_t > indexArray);
    std::vector < int16_t > matchBaFolaGlyph(std::vector < int16_t > indexArray);
    std::vector < int16_t > matchRaFolaGlyph(std::vector < int16_t > indexArray);
    std::vector < int16_t > mergeConjuctAndBaFola(std::vector < int16_t > indexArray);
    std::vector < int16_t > matchConjuctAKhandGlyph(std::vector < int16_t > indexArray);
    std::vector < int16_t > mergeConjuctAndRaFola(std::vector < int16_t > indexArray);
    std::vector < int16_t > matchTwoConsonantWithRaFolaGlyph(std::vector < int16_t > indexArray);
    std::vector < int16_t > matchTwoConsonantWithBaFolaGlyph(std::vector < int16_t > indexArray);

    std::vector < int16_t > convertUnicodeToIndex(std::vector < int16_t > unicodeArray);
    int16_t getIndexOfJaFola();
    int16_t getIndexOfRaFola();
    int16_t getIndexOfBaFola();
    int16_t getIndexOfRef();
    bool checkMarkIndex(int16_t index);

    int16_t findConjuctGlyphIndexFrom3Components(std::vector < int16_t > indexArray, std::string featureTag);
    int16_t findConjuctGlyphIndexFrom2Components(std::vector < int16_t > indexArray, std::string featureTag);

public:
    BTGlyphDecoder(BTAbstractUtility * helper);
    std::vector < int16_t > getGlyphIndexArrayFromText(std::string text);
};

#endif /* GlyphDecoder2_hpp */
