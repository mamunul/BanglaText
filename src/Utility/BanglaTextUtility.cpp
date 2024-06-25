/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#include <cstdint>
#include "BanglaTextUtility.hpp"

int16_t binarySearch(int16_t arr[], int16_t size, int16_t key)
{
    int16_t low = 0;
    int16_t high = size - 1;

    while (low <= high)
    {
        int16_t mid = low + (high - low) / 2;
        uint16_t array_mid = readWordFrom(arr, mid);

        if (array_mid == key)
        {
            return mid;
        }

        if (array_mid < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return -1;
}

int16_t binarySearch(char **arr, std::string key, int16_t size)
{
    int16_t l = 0;
    int16_t r = size - 1;

    while (l <= r)
    {
        int16_t m = l + (r - l) / 2;
        int16_t res = -1000;
        char *array_mid = readStringFromProgmem(arr,m);
        
        if (key == (array_mid))
        {
            res = 0;
        }

        if (res == 0)
        {
            return m;
        }

        if (key > (array_mid))
        {
            l = m + 1;
        }
        else
        {
            r = m - 1;
        }
    }

    return -1;
}

BTOffset BanglaTextUtility::findMarkBaseOffset(std::string key)
{
    int16_t i = 0;

    i = binarySearch(font->markbaseMapKeys, key, font->markbaseMapSize);

    if (i == -1)
    {
        return {0, 0};
    }

    BTOffset v = readStruct2FromProgmem(font->markbaseMapValues, i);

    return v;
}

int16_t BanglaTextUtility::findConjunct(std::string key, std::string tag)
{
    int16_t i = 0;

    i = binarySearch(font->conjunctMapKeys, key, font->conjunctMapSize);

    if (i == -1)
    {
        return i;
    }

    std::vector<BTConjunctGlyph> v = font->conjunctMapValues[i];

    if (v.size() == 1)
    {
        return v[0].index;
    }

    i = 0;

    i = binarySearch(font->conjunctTagMapKeys, tag, font->conjunctTagMapSize);

    for (auto e : v)
    {
        uint8_t rr = readByteFromProgmem(font->conjunctTagMapValues, i);
        
        if (e.tag == rr)
        {
            return e.index;
        }
    }

    return -1;
}

int16_t BanglaTextUtility::getGlyphUnicodeOfIndex(int16_t index)
{
    int16_t i = 0;
    int16_t terminatedValue = -1;

    for (int16_t *ptr = font->unicodeIndexMapValues; *ptr != terminatedValue; ptr++)
    {
        if (*ptr == index)
        {
            break;
        }

        i++;
    }

    if (font->unicodeIndexMapValues[i] == terminatedValue)
    {
        return -1;
    }

    return font->unicodeIndexMapKeys[i];

    return -1;
}

int16_t BanglaTextUtility::getGlyphIndexOfUnicode(int16_t code)
{
    int16_t i = 0;

    i = binarySearch(font->unicodeIndexMapKeys, font->unicodeIndexMapSize, code);
    if (i == -1)
    {
        return i;
    }
    uint16_t array_mid =  readWordFrom(font->unicodeIndexMapValues , i);
   
    return array_mid;
}

BanglaTextUtility::BanglaTextUtility(const BTfont *font)
{
    this->font = font;
}
