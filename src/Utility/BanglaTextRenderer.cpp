/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#include "BanglaTextRenderer.hpp"

#include <iostream>
#include <map>
#include <math.h>

#include "BanglaTextUtility.hpp"
#include "BTGlyphDecoder.hpp"

struct TextDimension
{
    int16_t w;
    int16_t h;
    int16_t minx;
    int16_t miny;
};

BanglaTextRenderer::BanglaTextRenderer(BTfont *font)
{
    this->font = font;
}

TextDimension measureTextDimension(std::vector<int16_t> glypharray, BTfont *font, BanglaTextUtility *helper)
{
    int16_t x = 0;
    int16_t y = 0;
    int16_t minx = 10000;
    int16_t maxx = -10000;
    int16_t miny = 10000;
    int16_t maxy = -10000;

    for (uint8_t i = 0; i < (uint8_t)glypharray.size(); ++i)
    {
        BTGlyph info = readStructFromProgmem(font->glyphArray, glypharray[i]);
        BTOffset off;

        off.x = 0;
        off.y = 0;

        if (i > 0)
        {
            std::string markbaseIndex = std::to_string(glypharray[i]) + "," + std::to_string(glypharray[i - 1]);
            off = helper->findMarkBaseOffset(markbaseIndex);
        }

        int16_t xa = info.xAdvance;
        int16_t ya = 0;
        int16_t xo = off.x;
        int16_t yo = off.y;

        int16_t x0 = x + xo + info.xOffset;
        int16_t y0 = floor(y + yo + info.yOffset);
        int16_t x1 = x0 + info.width;
        int16_t y1 = floor(y0 - info.height);

        minx = std::min(minx, x0);
        miny = std::min(miny, y1);

        maxx = std::max(maxx, x1);
        maxy = std::max(maxy, y0);

        x += xa;
        y += ya;
    }

    int16_t w = ceil(maxx - minx);
    int16_t h = ceil(maxy - miny);

    return {w, h, minx, miny};
}

#ifndef EMBEDDED
std::vector<std::string> glyphBitmapToBuffer(uint8_t *processed_bitmap, uint16_t start, uint16_t end, uint8_t width)
{
    uint16_t bitcount = 1;
    std::vector<std::string> buffer;
    std::string line;

    for (int i = start; i <= end; i++)
    {
        int byte = processed_bitmap[i];

        for (int j = 7; j >= 0; j--)
        {
            int bit = byte & (1 << j);

            if (bit)
            {
                line += "0";
            }
            else
            {
                line += " ";
            }

            if (bitcount % width == 0)
            {
                buffer.push_back(line);
                line = "";
            }

            bitcount++;
        }
    }

    return buffer;
}

void renderGlyphArrayInConsole(std::vector<int16_t> glypharray, BTfont *font, BanglaTextUtility *helper)
{

    auto size = measureTextDimension(glypharray, font, helper);
    int16_t w = size.w;
    int16_t h = size.h;

    bool verticalFlip = false;
    //
    std::vector<std::string> textBuffer = std::vector<std::string>(h, std::string(w, ' '));
    int16_t x = 0;
    int16_t y = 0;
    //
    for (int i = 0; i < (int)glypharray.size(); ++i)
    {
        BTGlyph info = font->glyphArray[glypharray[i]];

        BTOffset off;
        off.x = 0;
        off.y = 0;

        if (i > 0)
        {
            std::string markbaseIndex = std::to_string(glypharray[i]) + "," + std::to_string(glypharray[i - 1]);
            off = helper->findMarkBaseOffset(markbaseIndex);
        }
        //
        int16_t xa = info.xAdvance;
        int16_t ya = 0;
        int16_t xo = off.x;
        int16_t yo = off.y;

        int16_t x0 = x + xo + info.xOffset;
        int16_t y0 = floor(y + yo + info.yOffset);

        int16_t rx = ceil(x0) - size.minx;
        int16_t ry = h - (y0 - size.miny);

        uint16_t bitmap_start = info.bitmapOffset;
        uint16_t bitmap_end = font->glyphArray[glypharray[i] + 1].bitmapOffset - 1;

        std::vector<std::string> buffer = glyphBitmapToBuffer(font->bitmap, bitmap_start, bitmap_end, info.width);
        //
        for (int iy = 0; iy < info.height; ++iy)
        {
            for (int ix = 0; ix < info.width; ++ix)
            {
                if (buffer[iy][ix] == '0')
                {
                    if (verticalFlip)
                    {
                        textBuffer[h - 1 - ry - iy][rx + ix] = '0';
                    }
                    else
                    {
                        textBuffer[ry + iy][rx + ix] = '0';
                    }
                }
            }
        }
        x += xa;
        y += ya;
    }

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            std::cout << textBuffer[i][j];
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

void BanglaTextRenderer::renderTextInConsole(std::string text)
{
    BanglaTextUtility *helper = new BanglaTextUtility(font);
    BTGlyphDecoder *decoder = new BTGlyphDecoder(helper);
    std::vector<int16_t> indices = decoder->getGlyphIndexArrayFromText(text);
    renderGlyphArrayInConsole(indices, font, helper);
    //    cout << endl;
}
#endif

void renderGlyphArrayInDisplay(std::vector<int16_t> glypharray, BTfont *font, BanglaTextUtility *helper, void (*drawPixel)(int16_t, int16_t))
{
    int16_t x = 0;
    int16_t y = 0;
    auto size = measureTextDimension(glypharray, font, helper);
    //    int16_t w = size.w;
    int16_t h = size.h;
    
    for (uint8_t i = 0; i < (uint8_t)glypharray.size(); ++i)
    {
        BTGlyph info = readStructFromProgmem(font->glyphArray, glypharray[i]);
        ; //  font->glyphArray[glypharray[i]];

        BTOffset off;
        off.x = 0;
        off.y = 0;

        if (i > 0)
        {
            std::string markbaseIndex = std::to_string(glypharray[i]) + "," + std::to_string(glypharray[i - 1]);
            off = helper->findMarkBaseOffset(markbaseIndex);
        }

        float xa = (float)info.xAdvance;
        float ya = 0;
        float xo = off.x;
        float yo = off.y;

        float x0 = x + xo + info.xOffset;
        float y0 = floor(y + yo + info.yOffset);

        int16_t rx = ceil(x0) - size.minx;
        int16_t ry = h - (y0 - size.miny);

        uint16_t bitmap_start = info.bitmapOffset;
        BTGlyph info_next = readStructFromProgmem(font->glyphArray, glypharray[i] + 1);
        uint16_t bitmap_end = info_next.bitmapOffset - 1;

        uint16_t bitcount = 1;

        int16_t row = 0;
        int16_t column = 0;
        for (uint16_t i = bitmap_start; i <= bitmap_end; i++)
        {
            int byte = readByteFromProgmem(font->bitmap,i);// font->bitmap[i];

            for (int j = 7; j >= 0; j--)
            {
                int bit = byte & (1 << j);

                if (bit)
                {
                    drawPixel(h - 1 - ry - row, rx + column);
                }
                column++;

                if (bitcount % info.width == 0)
                {
                    row++;
                    column = 0;
                }

                bitcount++;
            }
        }

        x += xa;
        y += ya;
    }
    
}

BTTextSize BanglaTextRenderer::getTextSize(std::string text)
{
    BanglaTextUtility *helper = new BanglaTextUtility(font);
    BTGlyphDecoder *decoder = new BTGlyphDecoder(helper);
    std::vector<int16_t> indices = decoder->getGlyphIndexArrayFromText(text);
    auto size = measureTextDimension(indices, font, helper);
    BTTextSize ts = {size.w, size.h};
    return ts;
}

void BanglaTextRenderer::renderText(std::string text, void (*drawPixel)(int16_t, int16_t))
{
    BanglaTextUtility *helper = new BanglaTextUtility(font);
    BTGlyphDecoder *decoder = new BTGlyphDecoder(helper);
    std::vector<int16_t> indices = decoder->getGlyphIndexArrayFromText(text);

    renderGlyphArrayInDisplay(indices, font, helper, drawPixel);
}
