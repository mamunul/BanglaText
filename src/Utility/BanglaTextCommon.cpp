/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#include "BanglaTextCommon.hpp"

// Function to read an element from the array
BTGlyph readStructFromProgmem(BTGlyph *glyphArray, int16_t index)
{
    BTGlyph result;
#ifdef ESP8266
    memcpy_P(&result, &glyphArray[index], sizeof(BTGlyph));
#else
    result = glyphArray[index];
#endif
 
    // Ensure you read the correct amount of bytes for your struct

    return result;
}

BTOffset readStruct2FromProgmem(BTOffset *markbaseMapValues, int16_t index)
{
    BTOffset result;
    
#ifdef ESP8266
    memcpy_P(&result, &markbaseMapValues[index], sizeof(BTGlyph));
#else
    result =  markbaseMapValues[index];
#endif
    // Ensure you read the correct amount of bytes for your struct

    return result;
}

char * readStringFromProgmem(char **arr,int16_t index){
#ifdef ESP8266
        char *array_mid = (char *)pgm_read_dword(&(arr[index]));
#else
        char *array_mid =  arr[index];
#endif
    return array_mid;
}

uint8_t readByteFromProgmem(uint8_t *array, uint16_t index)
{
    // Use pgm_read_byte_near to read the data at the specified index
    
#ifdef ESP8266
    return pgm_read_byte_near(&array[index]);
#else
    return array[index];
#endif
}

uint16_t readWordFrom(int16_t *array, uint16_t index){
#ifdef ESP8266
    uint16_t array_mid = pgm_read_word_inlined(array + index);
#else
    uint16_t array_mid =  array[index];
#endif
    
    return array_mid;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim))
        result.push_back(item);

    return result;
}

uint16_t encoding2; // the unicode, detected by the utf-8 decoder
uint8_t utf8_state; // current state of the utf-8 decoder, contains the remaining bytes for a detected unicode glyph

uint16_t utf8_next2(uint8_t b) {
    if (b == 0) {       /* '\n' terminates the string to support the string list procedures */
        return 0x0ffff; /* end of string detected, pending UTF8 is discarded */
    }

    if (utf8_state == 0) {
        if (b >= 0xfc) { /* 6 byte sequence */
            utf8_state = 5;
            b &= 1;
        } else if (b >= 0xf8) {
            utf8_state = 4;
            b &= 3;
        } else if (b >= 0xf0) {
            utf8_state = 3;
            b &= 7;
        } else if (b >= 0xe0) {
            utf8_state = 2;
            b &= 15;
        } else if (b >= 0xc0) {
            utf8_state = 1;
            b &= 0x01f;
        } else {
            /* do nothing, just use the value as encoding */
            return b;
        }

        encoding2 = b;
        return 0x0fffe;
    } else {
        utf8_state--;
        /* The case b < 0x080 (an illegal UTF8 encoding) is not checked here. */
        encoding2 <<= 6;
        b &= 0x03f;
        encoding2 |= b;

        if (utf8_state != 0) {
            return 0x0fffe; /* nothing to do yet */
        }
    }

    return encoding2;
}

std::vector<int16_t> BTAbstractUtility::decodeStringToUnicode(std::string text) {
    const char *str = text.c_str();
    std::vector<int16_t> vs;
    uint16_t e;

    for (;;) {
        e = utf8_next2((uint8_t)*str);

        if (e == 0x0ffff) {
            break;
        }

        str++;

        if (e != 0x0fffe) {
            vs.push_back(e);
        }
    }

    return vs;
}
