/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#include <string>
#include "BanglaTextRenderer.hpp"
#include "CairoWrapper.hpp"
#include "FontDataExtractor.hpp"
#include "FontHeaderCreator.hpp"
#include "FreetypeWrapper.hpp"
#include "HarfbuzzWrapper.hpp"
#include "TextConsoleRenderer.hpp"
#include "TextImageExporter.hpp"

#include "common.hpp"
#include "Header.h"

#include "BanglaMNRegular_25pt.h"
#include "BanglaMNRegular_21pt.h"
#include "BanglaMNRegular_33pt.h"

#include "SangamMN_21pt.h"
#include "SangamMN_25pt.h"
#include "SangamMN_31pt.h"

#include "Kalpurush_20pt.h"
#include "Kalpurush_25pt.h"
#include "Kalpurush_33pt.h"

#include "Kohinur_22pt.h"
#include "Kohinur_25pt.h"
#include "Kohinur_30pt.h"

using namespace std;

//#define DPI 141
//#define DPI 100 //epaper disaply640x384 - 7.5"
#define DPI 72 //working only for harfbuzz image generation code

/*
   This method is to render the words with all bengali conjucts in console with the help of freetype and harfbuzz
   library to check the data taken from those libraries and the algorithm used to render the text is ok.
 */
void renderAllConjunctWordstInConsole(
    FreetypeWrapper        *freetypeWrapper,
    BTGlyphDecoder         *glyphProcessor,
    HarfBuzzWrapper        *hbWrapper,
    NameIndexUnicodeHelper *helper
    ) {
    TextConsoleRenderer *tr = new TextConsoleRenderer(freetypeWrapper, glyphProcessor, hbWrapper, helper);
    vector<string> data = split(all_conjuct_words, ' ');

    for (auto text:data) {
        cout << text << endl;
        tr->logText(text);
    }
}

/*
   This method is to render text in console with the help of freetype and harfbuzz library to check the data taken from
   those libraries and the algorithm used to render the text is ok.
 */
void renderTextInConsole(
    FreetypeWrapper        *freetypeWrapper,
    BTGlyphDecoder         *glyphProcessor,
    HarfBuzzWrapper        *hbWrapper,
    NameIndexUnicodeHelper *helper
    ) {
    TextConsoleRenderer *tr = new TextConsoleRenderer(freetypeWrapper, glyphProcessor, hbWrapper, helper);
    string text;

    text = "ইক্ষ্বাকু";
    text = "দোগ্ধ্রী";
    tr->logText(text);
}

/*
   This method is to render the words with all bengali conjucts in console by using the created font header to check
   the data stored in the header and the algorithm used to render the text is ok.
 */
void renderAllConjunctWordsWithFontHeaderLib() {
    BTfont font;

    font = BanglaMNRegular_25pt;
//    font = BanglaMNRegular_21pt;
//    font = BanglaMNRegular_33pt;
    
//    font = SangamMN_21pt;
//    font = SangamMN_25pt;
//    font = SangamMN_31pt;
    
//    font = Kalpurush_20pt;
//    font = Kalpurush_25pt;
//    font = Kalpurush_33pt;
    
//    font = Kohinur_22pt;
//    font = Kohinur_25pt;
//    font = Kohinur_30pt;

    BanglaTextRenderer *display = new BanglaTextRenderer(&font);
    vector<string> data = split(all_conjuct_words, ' ');

    for (auto text:data) {
        cout << text << endl;
        display->renderTextInConsole(text);
    }
}

/*
   This method is to render text in console by using the created font header to check the data stored in the header and
   the algorithm used to render the text is ok.
 */
void renderTextWithFontHeaderLib() {
    BTfont font;

    font = BanglaMNRegular_25pt;
    //    font = BanglaMNRegular_21pt;
    //    font = BanglaMNRegular_33pt;
        
    //    font = SangamMN_21pt;
    //    font = SangamMN_25pt;
    //    font = SangamMN_31pt;
        
    //    font = Kalpurush_20pt;
    //    font = Kalpurush_25pt;
    //    font = Kalpurush_33pt;
        
    //    font = Kohinur_22pt;
    //    font = Kohinur_25pt;
    //    font = Kohinur_30pt;

    BanglaTextRenderer *display = new BanglaTextRenderer(&font);

    string text;
    text = "ইক্ষ্বাকু";
    text = "দোগ্ধ্রী";
    auto size = display->getTextSize(text);
    display->renderTextInConsole(text);
}

void exportGlyphAsImage(
    CairoWrapper *cairoWrapper,
    NameIndexUnicodeHelper *helper,
    map < string, vector < OutGlyph > > allConjuncts,
    map < string, BTOffset > allMarkPos
    ) {
    TextImageExporter *imageExporter = new TextImageExporter(cairoWrapper, helper, allConjuncts, allMarkPos);

    imageExporter->renderAllConjuncts();
//    imageExporter->renderAllMarkBase();
}

/*
   This method generates font header file, necessary to render bangla text in embedded display. This header can be
   tested in here as well by using the method renderTextWithFontHeaderLib.
 */
void generateFontHeader(
    string                 fontName,
    FreetypeWrapper        *freetypeWrapper,
    FontDataExtractor      *fontExtractor,
    HarfBuzzWrapper        *harfbuzzWrapper,
    NameIndexUnicodeHelper *helper
    ) {
    FontHeaderCreator creator = FontHeaderCreator(fontName, freetypeWrapper, fontExtractor, harfbuzzWrapper, helper);

    string filePath = "../FontsHeader/" + fontName + ".h";

    creator.generateHeaderFile(filePath);
}

int main(int argc, const char *argv[]) {
    FontInfo fontInfo = getbanglaMNFont();

    FreetypeWrapper *freetypeWrapper = new FreetypeWrapper(fontInfo, DPI);

    FontDataExtractor *fontExtractor = new FontDataExtractor(fontInfo);
    map < string, vector < OutGlyph > > allConjuncts = fontExtractor->getAllConjuncts();
    map < uint16_t, uint16_t > unicodeIndexMap = fontExtractor->getUnicodeIndexMap();
    map < string, BTOffset > allMarkPos = fontExtractor->getAllMarkBaseOffset();

    NameIndexUnicodeHelper *helper = new NameIndexUnicodeHelper(allConjuncts, unicodeIndexMap);
    BTGlyphDecoder *glyphProcessor = new BTGlyphDecoder(helper);

    HarfBuzzWrapper *harfbuzzWrapper = new HarfBuzzWrapper(fontInfo, DPI);
    CairoWrapper *cairoWrapper = new CairoWrapper(harfbuzzWrapper, fontInfo, DPI);

//    renderTextInConsole(freetypeWrapper, glyphProcessor, harfbuzzWrapper, helper);
//
//    exportGlyphAsImage(cairoWrapper, helper, allConjuncts, allMarkPos);

    string fontName = fontInfo.fontName + "_" + to_string(fontInfo.fontSize) + "pt";

//    generateFontHeader(fontName, freetypeWrapper, fontExtractor, harfbuzzWrapper, helper);

    renderTextWithFontHeaderLib();
//    renderAllConjunctWordsWithFontHeaderLib();

    return 0;
}
