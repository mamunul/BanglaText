/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#ifndef Helper2_hpp
#define Helper2_hpp

#include <map>
#include <map>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include "BanglaTextCommon.hpp"

// using namespace std;

class BanglaTextUtility: public BTAbstractUtility {
private:
    const BTfont *font;

public:
    BanglaTextUtility(const BTfont *font);

    int16_t getGlyphIndexOfUnicode(int16_t code) override;
    int16_t getGlyphUnicodeOfIndex(int16_t index) override;
    int16_t findConjunct(std::string key, std::string tag) override;
    BTOffset findMarkBaseOffset(std::string key);
};


#endif /* Helper2_hpp */
