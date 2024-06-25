/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#include <cctype>
#include <cstdint>
#include <format>
#include <fstream>
#include <set>
#include "common.hpp"
#include "FontHeaderCreator.hpp"
#include "HeaderStrings.h"

FontHeaderCreator::FontHeaderCreator(
    string                 fontName,
    FreetypeWrapper        *freetype,
    FontDataExtractor      *extractor,
    HarfBuzzWrapper        *hb,
    NameIndexUnicodeHelper *helper
    )
    : freetype(freetype), extractor(extractor), hb(hb), helper(helper) {
    this->fontName = fontName;
}

struct VariableData {
    string content;
    uint16_t arraySize;
    uint16_t allocatedByte;
};

map<string, uint8_t> getAllTags(map < string, vector < OutGlyph > > conjunctMap) {
    set<string> tagset; map<string, uint8_t> tagmap;

    for (auto it = conjunctMap.begin(); it != conjunctMap.end(); ++it) {
        for (int j = 0; j < it->second.size(); j++) {
            tagset.insert(it->second[j].tag);
        }
    }

    uint8_t i = 0;

    for (auto it:tagset) {
        tagmap[it] = i++;
    }

    return tagmap;
}

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args) {
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;   // Extra space for '\0'

    if (size_s <= 0) {
        throw std::runtime_error("Error during formatting.");
    }

    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[ size ]);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1);   // We don't want the '\0' inside
}

VariableData generate_bitmap_array(string fontName, vector<uint8_t>    bitmap_data) {
    string bitmapstr;

    bitmapstr = string_format(bitmapDefStr, fontName.c_str());

    for (long i = 0; i < bitmap_data.size(); i++) {
        bitmapstr += string_format("0x%02X", bitmap_data[i]);

        if (i != bitmap_data.size() - 1) {
            bitmapstr += ", ";
        }

        if ((i + 1) % 16 == 0) {
            bitmapstr += "\n";
        }
    }

    bitmapstr += " };\n\n"; // End bitmap array
    uint16_t byteSize = (uint16_t)bitmap_data.size();
    bitmapstr += string_format("//BitmapByte = %dBytes\n", byteSize);

    VariableData vd = { bitmapstr, (uint16_t)bitmap_data.size(), byteSize };

    return vd;
}

VariableData generate_glyph_properties(string fontName, vector<Glyph_Info> table) {
    string glyphStr;

    glyphStr = string_format(glyphDefStr, fontName.c_str());

    int bitmap_offset = 0;

    for (int i = 0, j = 0; i < table.size(); i++, j++) {
        glyphStr += string_format("{ %5d, %3d, %3d, %3d, %4d, %4d }", bitmap_offset,
                                  table[j].width, table[j].height, (int)table[j].x_advance, table[j].x_offset,
                                  table[j].y_offset);

        if (i < table.size()) {
            glyphStr +=  string_format(",   // %d", table[i].gid);

            if ((i >= ' ') && (i <= '~')) {
                glyphStr += string_format(" '%c'", i);
            }

            glyphStr += "\n";
        }

        bitmap_offset += table[j].processed_bitmap.size();
    }

    int byteSize = (int)table.size() * 7;
    glyphStr += " }; // ";

    glyphStr +=  string_format("// Total Size = %dBytes\n", byteSize);

    glyphStr += "\n\n";
    VariableData vd = { glyphStr, (uint16_t)table.size(), (uint16_t)byteSize };
    return vd;
}

VariableData generateUnicodeIndexMap(string fontName, map < uint16_t, uint16_t > unicodeIndexMap) {
    string unicodeIndexKeyString, unicodeIndexValueString;

    unicodeIndexKeyString = string_format(unicodeIndexMapkeysDefStr, fontName.c_str());
    unicodeIndexValueString = string_format(unicodeIndexMapValuesDefStr, fontName.c_str());
    int i = 0;

    bool lastCommaPlaced = false;

    for (auto it = unicodeIndexMap.begin(); it != unicodeIndexMap.end(); ++it) {
        i++;

        if (it->first >= numeric_limits<int16_t>::max() || it->second >= numeric_limits<int16_t>::max()) {
            continue;
        }

        lastCommaPlaced = false;
        unicodeIndexKeyString += string_format("%d", it->first);
        unicodeIndexValueString += string_format("%d", it->second);

        if (std::next(it) != unicodeIndexMap.end()) {
            unicodeIndexKeyString +=  ",";
            unicodeIndexValueString +=  ",";
            lastCommaPlaced = true;
        }

        if ((i + 1) % 6 == 0) {
            unicodeIndexKeyString += "\n";
            unicodeIndexValueString += "\n";
        }
    }

    unicodeIndexKeyString += lastCommaPlaced ? "-1" : ",-1";
    unicodeIndexValueString += lastCommaPlaced ? "-1" : ",-1";
    unicodeIndexKeyString += " };\n\n"; // End bitmap array
    unicodeIndexValueString += " };\n\n"; // End bitmap array
    int byteSize = (int)unicodeIndexMap.size() * 4;

    string content = unicodeIndexKeyString + unicodeIndexValueString;
    content +=   string_format("// unicodeIndexMap size = %dBytes\n", byteSize);

    VariableData vd = { content, (uint16_t)unicodeIndexMap.size(),  (uint16_t)byteSize };

    return vd;
}

VariableData generateMarkBaseOffset(string fontName, map < string, BTOffset > markbaseMap) {
    string markbaseKeyStr, valuStr;

    markbaseKeyStr = string_format(markBaseKeyDefStr, fontName.c_str());
    valuStr = string_format(markBaseValueDefStr, fontName.c_str());
    int i = 0; int byteSize = 0;

    for (auto it = markbaseMap.begin(); it != markbaseMap.end(); ++it) {
        i++;
        markbaseKeyStr +=  string_format("\"%s\"", it->first.c_str());
        byteSize += it->first.length();

        valuStr += string_format("{%d,%d}", it->second.x, it->second.y);
        byteSize += 2;

        if (std::next(it) != markbaseMap.end()) {
            markbaseKeyStr += ",";
            valuStr += ",";
        }

        if ((i + 1) % 6 == 0) {
            valuStr += "\n";
            markbaseKeyStr += "\n";
        }
    }

    markbaseKeyStr += ",NULL";
    valuStr += "};\n\n";
    markbaseKeyStr += "};\n\n";

    string content = markbaseKeyStr + valuStr;
    content += string_format("//approximate markBase size = %dBytes\n", byteSize);

    VariableData vd = { content, (uint16_t)markbaseMap.size(), (uint16_t)byteSize };

    return vd;
}

VariableData generateConjunctTagMap(string fontName, map<string, uint8_t> tagMap) {
    string tagmapKeyStr = string_format(conjunctTagMapKeysDefStr, fontName.c_str());
    string tagmapValueStr = string_format(conjunctTagMapValueDefStr, fontName.c_str());
    int j = 0;     int byteSize = 0;

    for (auto it = tagMap.begin(); it != tagMap.end(); ++it) {
        tagmapKeyStr += string_format("\"%s\"", it->first.c_str());
        tagmapValueStr += string_format("%d", it->second);
        j++;

        byteSize += it->first.length();
        byteSize += 1;//for index

        if (std::next(it) != tagMap.end()) {
            tagmapKeyStr += ",";
            tagmapValueStr += ",";
        }
    }

    tagmapKeyStr += ",NULL";
    tagmapValueStr += ",-1";
    tagmapKeyStr += "};\n\n";
    tagmapValueStr += "};\n\n";
    string tagmapStr = tagmapKeyStr + tagmapValueStr;

    VariableData vd = { tagmapStr, (uint16_t)tagMap.size(), (uint16_t)byteSize };

    return vd;
}

VariableData generateConjunctMap(string fontName, map < string, vector < OutGlyph > > conjunctMap, map<string, uint8_t> tagMap) {
    string conjunctKeyString, conjunctValueStr;

    conjunctKeyString = string_format(conjunctMapKeyDefStr, fontName.c_str());
    conjunctValueStr = string_format(conjunctMapValueDefStr, fontName.c_str());

    int byteSize = 0;

    int i = 0;

    for (auto it = conjunctMap.begin(); it != conjunctMap.end(); ++it) {
        i++;
        conjunctKeyString += string_format("\"%s\"", it->first.c_str());
        byteSize += it->first.length();

        conjunctValueStr += "{";

        for (int j = 0; j < it->second.size(); j++) {
            conjunctValueStr += string_format("{%d,%d}", tagMap[it->second[j].tag], it->second[j].index);
            byteSize += it->second[j].tag.length();
            byteSize += 2;//for index

            if (j != it->second.size() - 1) {
                conjunctValueStr += ",";
            }
        }

        conjunctValueStr += "}";

        if (std::next(it) != conjunctMap.end()) {
            conjunctKeyString += ",";
            conjunctValueStr += ",";
        }

        if ((i + 1) % 6 == 0) {
            conjunctKeyString += "\n";
            conjunctValueStr += "\n";
        }
    }

    conjunctKeyString += ",NULL";
    conjunctValueStr += " };\n\n"; // End bitmap array
    conjunctKeyString += " };\n\n"; // End bitmap array

    string content = conjunctKeyString + conjunctValueStr;

    content += string_format("//approximate conjunctMap size = %dBytes\n", byteSize);

    VariableData vd = { content, (uint16_t)conjunctMap.size(), (uint16_t)byteSize };

    return vd;
}

vector<uint8_t> accumulateAllGlyphBitmap(vector<Glyph_Info> glyphArray) {
    vector<uint8_t> bitmap_data;

    for (int i = 0; i < glyphArray.size(); i++) {
        auto glyphBitmap = glyphArray[i].processed_bitmap;
        bitmap_data.insert(bitmap_data.end(), glyphBitmap.begin(), glyphBitmap.end());
    }

    return bitmap_data;
}

map < string, BTOffset > collectMarkBaseOffset(
    map < string, BTOffset > markbaseMap,
    HarfBuzzWrapper *hb,
    NameIndexUnicodeHelper *helper
    ) {
    map < string, BTOffset > filteredMarkbaseMap;

    for (auto it = markbaseMap.begin(); it != markbaseMap.end(); ++it) {
        auto indices = split(it->first, ',');
        int markIndex = stoi(indices[0]);
        int baseIndex = stoi(indices[1]);
        vector<int> unicodeArray = helper->glyphIndexToUnicodeCharArray(baseIndex);
        int markUnicode = helper->getGlyphUnicodeOfIndex(markIndex);
        
        if(markUnicode == -1) 
            continue;
        unicodeArray.push_back(markUnicode);
        auto propertyArray = hb->get_glyph_properties(unicodeArray);
        markbaseMap[it->first].x = propertyArray[1].x_offset;
        markbaseMap[it->first].y = propertyArray[1].y_offset;

        bool invalid = false;

        for (int code:unicodeArray) {
            if (code == -1) {
                invalid = true;
            }
        }

        if (propertyArray[1].x_offset == 0 && propertyArray[1].y_offset == 0) {
            invalid = true;
        }

        if (!invalid) {
            filteredMarkbaseMap[it->first] = BTOffset{ (int8_t)propertyArray[1].x_offset, (int8_t)propertyArray[1].y_offset };
        }
    }

    return filteredMarkbaseMap;
}

pair<string, int> FontHeaderCreator::getFontHeaderContent() {
    vector<Glyph_Info> glyphArray = freetype->getAllGlyph();
    map < string, vector < OutGlyph > > conjunctMap = extractor->getAllConjuncts();
    map < uint16_t, uint16_t > unicodeIndexMap = extractor->getUnicodeIndexMap();
    map < string, BTOffset > markbaseMap = extractor->getAllMarkBaseOffset();
    auto tagmap = getAllTags(conjunctMap);

    markbaseMap = collectMarkBaseOffset(markbaseMap, hb, helper);
    vector<uint8_t> bitmap_data = accumulateAllGlyphBitmap(glyphArray);

    auto glyph = generate_glyph_properties(fontName, glyphArray);
    auto bitmap = generate_bitmap_array(fontName, bitmap_data);
    auto unicodeIndex = generateUnicodeIndexMap(fontName, unicodeIndexMap);
    auto conjunct = generateConjunctMap(fontName, conjunctMap, tagmap);
    auto markBase = generateMarkBaseOffset(fontName, markbaseMap);
    auto tagMapContent = generateConjunctTagMap(fontName, tagmap);

    string content =  glyph.content + bitmap.content + unicodeIndex.content + tagMapContent.content + conjunct.content + markBase.content;

    int byteSize = 0;
    byteSize += glyph.allocatedByte;
    byteSize += bitmap.allocatedByte;
    byteSize += unicodeIndex.allocatedByte;
    byteSize += conjunct.allocatedByte;
    byteSize += markBase.allocatedByte;
    byteSize += tagMapContent.allocatedByte;
    content = includeStr + content + "\n";

    content += string_format(fontDefStr, fontName.c_str());

    content += string_format(fontArgStr,
                             fontName.c_str(),
                             fontName.c_str(),
                             fontName.c_str(),
                             fontName.c_str(),
                             fontName.c_str(),
                             fontName.c_str(),
                             fontName.c_str(),
                             fontName.c_str(),
                             fontName.c_str(),
                             fontName.c_str(),
                             unicodeIndex.arraySize,
                             conjunct.arraySize,
                             markBase.arraySize,
                             glyph.arraySize,
                             bitmap.arraySize,
                             tagMapContent.arraySize
                             );

    return make_pair(content, byteSize);
}

void FontHeaderCreator::consoleLogHeaderFile() {
    auto result = getFontHeaderContent();

    cout << result.first << endl;
    cout << endl << "// Total Byte:" << result.second << endl;
}

void FontHeaderCreator::generateHeaderFile(string filePath) {
    if (filePath.find(".h") != string::npos) {
        auto result = getFontHeaderContent();

        ofstream myfile;
        myfile.open(filePath);
        myfile << result.first << "\n";
        myfile << "//Total " << result.second << " Bytes\n";
        myfile.close();
    }
}
