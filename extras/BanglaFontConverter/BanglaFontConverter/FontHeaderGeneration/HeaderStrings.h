/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#ifndef HeaderStrings_h
#define HeaderStrings_h

#include <string>

std::string progmemStr = "\n #ifdef EMBEDDED \n PROGMEM \n #endif \n";

std::string glyphDefStr = "const BTGlyph %s_Glyphs[] " + progmemStr + "= {\n";
std::string unicodeIndexMapkeysDefStr = "const int16_t %s_unicodeIndexMapKeys[]  " + progmemStr + "= {\n";
std::string unicodeIndexMapValuesDefStr = "const int16_t %s_unicodeIndexMapValues[]  " + progmemStr + "= {\n";
std::string bitmapDefStr = "const uint8_t %s_Bitmaps[]  " + progmemStr + "= {\n";
std::string markBaseKeyDefStr = "static const char * %s_markbaseKey[]  " + progmemStr + "= {\n";
std::string markBaseValueDefStr = "const BTOffset %s_markbaseValue[]  " + progmemStr + "= {\n";
std::string conjunctMapKeyDefStr = "static const char * %s_conjunctMapKey[]  " + progmemStr + "= {\n";
std::string conjunctMapValueDefStr = "const std::vector<BTConjunctGlyph> %s_conjunctMapValue[]  = {\n";
std::string conjunctTagMapKeysDefStr = "static const char * %s_conjunctTagMapKeys[] " + progmemStr + " = {\n";
std::string conjunctTagMapValueDefStr = "const int8_t %s_conjunctTagMapValues[] " + progmemStr + " = {\n";
std::string fontDefStr = "const BTfont %s \n #ifdef EMBEDDED \n PROGMEM \n #endif \n = {";
std::string fontArgStr = " \n\
        (uint8_t * )%s_Bitmaps, \n\
        (BTGlyph * )%s_Glyphs, \n\
        (int16_t *) %s_unicodeIndexMapKeys, \n\
        (int16_t *) %s_unicodeIndexMapValues, \n\
        (char **) %s_conjunctTagMapKeys, \n\
        (uint8_t *) %s_conjunctTagMapValues, \n\
        (char **  )%s_conjunctMapKey, \n\
        (std::vector<BTConjunctGlyph> * )%s_conjunctMapValue, \n\
        (char **  )%s_markbaseKey, \n\
        (BTOffset * )%s_markbaseValue, \n\
        %u, // unicodeIndexMapSize \n\
        %u, // conjunctMapSize \n\
        %u, // markbaseMapSize \n\
        %u, // glyphArraySize \n\
        %u,  // bitmapSize  \n\
        %u // conjunctTagMapSize \n\
        };\n";

std::string includeStr = "#include <map> \n\
#include <string> \n\
#include <vector> \n\
\n";
#endif /* HeaderStrings_h */
