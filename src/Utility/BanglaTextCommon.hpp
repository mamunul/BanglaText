/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#ifndef lib_hpp
#define lib_hpp

#include <stdio.h>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#ifdef ESP8266
#include <pgmspace.h>
#endif

// using namespace std;

struct BTConjunctGlyph
{
    uint8_t tag;
    int16_t index;
};

struct BTOffset
{
    int8_t x;
    int8_t y;
};

typedef struct {
  uint16_t bitmapOffset; ///< Pointer into GFXfont->bitmap
  uint8_t width;         ///< Bitmap dimensions in pixels
  uint8_t height;        ///< Bitmap dimensions in pixels
  uint8_t xAdvance;      ///< Distance to advance cursor (x axis)
  int8_t xOffset;        ///< X dist from cursor pos to UL corner
  int8_t yOffset;        ///< Y dist from cursor pos to UL corner
} BTGlyph;

typedef struct
{
    uint8_t *bitmap;
    BTGlyph *glyphArray;
    int16_t * unicodeIndexMapKeys;
    int16_t * unicodeIndexMapValues;
    char ** conjunctTagMapKeys;
    uint8_t *conjunctTagMapValues;
    char ** conjunctMapKeys;
    std::vector<BTConjunctGlyph> *conjunctMapValues;
    char ** markbaseMapKeys;
    BTOffset *markbaseMapValues;
    uint16_t unicodeIndexMapSize;
    uint16_t conjunctMapSize;
    uint16_t markbaseMapSize;
    uint16_t glyphArraySize;
    uint16_t bitmapSize;
    uint16_t conjunctTagMapSize;
} BTfont;

struct BTTextSize{
    int16_t width;
    int16_t height;
};


enum BTUnicodeChar
{
    AKar = 2494,         //
    EKar = 2503,         //
    OKar = 2507,         //
    OuKar = 2508,        //
    AuKar = 2519,        //
    OiKar = 2504,        //
    IKar = 2495,         //
    Virama = 2509,       //
    UVowel = 2497,       // ◌ু
    UuVowel = 2498,      // ◌ূ
    RVowel = 2499,       // ◌ৃ
    IiVowel = 2496,      //
    Ka = 2453,           //
    MurdhannoSha = 2487, // ষ
    BorgioJa = 2460,     // জ
    Nya = 2462,          // ঞ
    Ja = 2479,           // য
    Ra = 2480,           //
    Ba = 2476,           //
    RaMiddle = 2544,     //
    KhandaTa = 2510,     //
    Nukta = 2492         //
};

std::vector<std::string> split(const std::string &s, char delim);
uint8_t readByteFromProgmem(uint8_t *array, uint16_t index);
uint16_t readWordFrom(int16_t *array, uint16_t index);
char * readStringFromProgmem(char **arr,int16_t index);
BTOffset readStruct2FromProgmem(BTOffset *markbaseMapValues, int16_t index);
BTGlyph readStructFromProgmem(BTGlyph *glyphArray, int16_t index);

class BTAbstractUtility {
public:
    virtual int16_t getGlyphIndexOfUnicode(int16_t code) = 0; // Pure virtual function makes this class abstract
    virtual  int16_t getGlyphUnicodeOfIndex(int16_t index) = 0;
    virtual int16_t findConjunct(std::string key, std::string tag) = 0;
    std::vector<int16_t> decodeStringToUnicode(std::string text);
    virtual ~BTAbstractUtility() {} // Virtual destructor
};

#endif /* lib_hpp */
