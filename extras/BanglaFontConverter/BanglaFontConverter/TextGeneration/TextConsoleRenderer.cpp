/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#include "common.hpp"
#include "TextConsoleRenderer.hpp"

TextConsoleRenderer::TextConsoleRenderer(
    FreetypeWrapper        *freetypepro,
    BTGlyphDecoder         *glyphProcessor,
    HarfBuzzWrapper        *hbwrapper,
    NameIndexUnicodeHelper *helper
    ) : freetypepro(freetypepro), glyphProcessor(glyphProcessor), hbwrapper(hbwrapper), helper(helper) {
}

vector<HBGlyphProperty> getMarkBaseGlyphInfo(
    NameIndexUnicodeHelper *helper,
    HarfBuzzWrapper        *hbwrapper,
    int                    markIndex,
    int                    baseIndex
    ) {
    vector<int> unicodeIntArray = helper->glyphIndexToUnicodeCharArray(baseIndex);

    unicodeIntArray.push_back(baseIndex);
    vector<HBGlyphProperty> array = hbwrapper->get_glyph_properties(unicodeIntArray);

    return array;
}

vector<string> glyphBitmapToBuffer(vector<uint8_t> processed_bitmap, int width) {
    int bitcount = 1;
    vector<string> buffer; string line;

    for (int i = 0; i <  processed_bitmap.size(); i++) {
        int byte = processed_bitmap[i];

        for (int j = 7; j >= 0; j--) {
            int bit = byte & (1 << j);

            if (bit) {
                line += "0";
            } else {
                line += " ";
            }

            if (bitcount % width == 0) {
                buffer.push_back(line);
                line = "";
            }

            bitcount++;
        }
    }

    return buffer;
}

void TextConsoleRenderer::logText(string text) {
    vector<int16_t> indexArray = glyphProcessor->getGlyphIndexArrayFromText(text);
    bool render = true;

    for (int i = 0; i < indexArray.size(); i++) {
        if (indexArray[i] <= 0) {
            cout << "rendering error: " << text << endl;
            render = false;
            break;
        }
    }

    if (render) {
        renderGlyphArray(indexArray);
    }

    cout << endl;
}

bool TextConsoleRenderer::checkMarkIndex(int16_t index) {
    int code = helper->getGlyphUnicodeOfIndex(index);

    if (code == UVowel || code == UuVowel || code == RVowel) {
        return true;
    }

    return false;
}

void TextConsoleRenderer::renderGlyphArray(vector<int16_t> glypharray) {
    vector<string> buffer;

    float minx = 10000;
    float maxx = -10000;
    float miny = 10000;
    float maxy = -10000;

    int x = 0; int y = 0;

    for (int i = 0; i < glypharray.size(); ++i) {
        Glyph_Info info =  freetypepro->getGlyph(glypharray[i]);

        vector<HBGlyphProperty> property;

        if (i > 0) {
//        if (i > 0 && checkMarkIndex(glypharray[i])) {
            property = getMarkBaseGlyphInfo(helper, hbwrapper, glypharray[i], glypharray[i - 1]);
        }

        float xa = (float)info.x_advance;
        float ya = (float)info.y_advance;
        float xo =  property.size() > 0 ? (float)property[1].x_offset : 0;
        float yo = property.size() > 0 ? (float)property[1].y_offset : 0;

        float x0 = x + xo + info.bearing_x;
        float y0 = floor(y + yo + info.bearing_y);
        float x1 = x0 + info.width;
        float y1 = floor(y0 - info.height);


        minx = min(minx, x0);
        miny = min(miny, y1);

        maxx = max(maxx, x1);
        maxy = max(maxy, y0);

        x += xa;
        y += ya;
    }

    int w = ceil(maxx - minx);
    int h = ceil(maxy - miny);

    vector<string> textBuffer = vector<string>(h, string(w, ' '));
    x = 0; y = 0;

    for (int i = 0; i < glypharray.size(); ++i) {
        Glyph_Info info =  freetypepro->getGlyph(glypharray[i]);
        vector<HBGlyphProperty> property;

        if (i > 0 && checkMarkIndex(glypharray[i])) {
            property = getMarkBaseGlyphInfo(helper, hbwrapper, glypharray[i], glypharray[i - 1]);
        }

        float xa = (float)info.x_advance;
        float ya = (float)info.y_advance;
        float xo =  property.size() > 0 ? (float)property[1].x_offset : 0;
        float yo = property.size() > 0 ? (float)property[1].y_offset : 0;

        float x0 = x + xo + info.bearing_x;
        float y0 = floor(y + yo + info.bearing_y);

        int rx = ceil(x0) - minx;
        int ry = h - (y0 - miny);

        vector<string> buffer = glyphBitmapToBuffer(info.processed_bitmap, info.width);

        for (int iy = 0; iy < info.height; ++iy) {
            for (int ix = 0; ix < info.width; ++ix) {
                if (buffer[iy][ix] == '0') {
                    textBuffer[ry + iy][rx + ix] = '0';
                }
            }
        }

        x += xa;
        y += ya;
    }

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            cout << textBuffer[i][j];
        }

        cout << endl;
    }

    cout << endl;
}
