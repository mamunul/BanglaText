/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#ifndef TextDisplay_hpp
#define TextDisplay_hpp

#include <stdio.h>
#include "BanglaTextCommon.hpp"

class BanglaTextRenderer
{
    BTfont *font;
public:
    BanglaTextRenderer(BTfont * font);
#ifndef EMBEDDED
    void renderTextInConsole(std::string text);
#endif
    BTTextSize getTextSize(std::string text);
    void renderText(std::string text, void (*drawPixel)(int16_t, int16_t));
};

#endif /* TextDisplay_hpp */
