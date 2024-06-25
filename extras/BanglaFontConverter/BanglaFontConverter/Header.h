/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#ifndef Header_h
#define Header_h

#include "common.hpp"

FontInfo getKalpurushFont() {
    FontInfo kalpurushFont = FontInfo();

    kalpurushFont.fontSize = 26;
    kalpurushFont.filepathname =  "../Fonts/kalpurush.ttf";
    kalpurushFont.fontName = "Kalpurush";
    kalpurushFont.gsubFile = "../FontsProcessed/kalpurush/kalpurush_gsub.json";
    kalpurushFont.gposFile = "../FontsProcessed/kalpurush/kalpurush_gpos.json";
    kalpurushFont.nameMapFile = "../FontsProcessed/kalpurush/kalpurush_nameMap.json";
    kalpurushFont.unicodeMapFile = "../FontsProcessed/kalpurush/kalpurush_unicodeName.json";

    return kalpurushFont;
}

FontInfo getbanglaMNFont() {
    FontInfo bmn = FontInfo();
    
    bmn.fontSize = 25;
    bmn.filepathname =  "../Fonts/BanglaMN.ttf";
    bmn.fontName = "BanglaMNRegular";
    bmn.gsubFile = "../FontsProcessed/bmn/bmn_gsub.json";
    bmn.gposFile = "../FontsProcessed/bmn/bmn_gpos.json";
    bmn.nameMapFile = "../FontsProcessed/bmn/bmn_nameMap.json";
    bmn.unicodeMapFile = "../FontsProcessed/bmn/bmn_unicodeName.json";
    
    return bmn;
}
FontInfo getSangamFont() {
    FontInfo sangam = FontInfo();
    
    sangam.fontSize = 26;
    sangam.filepathname =  "../Fonts/BanglaSangamMN.ttf";
    sangam.fontName = "SangamMN";
    sangam.gsubFile = "../FontsProcessed/sangam/bsangammn_gsub.json";
    sangam.gposFile = "../FontsProcessed/sangam/bsangammn_gpos.json";
    sangam.nameMapFile = "../FontsProcessed/sangam/bsangammn_nameMap.json";
    sangam.unicodeMapFile = "../FontsProcessed/sangam/bsangammn_unicodeName.json";
    return sangam;
}
FontInfo getKohinurFont() {
    FontInfo kohinur = FontInfo();
    
    kohinur.fontSize = 26;
    kohinur.filepathname =  "../Fonts/KohinoorBanglaRegular.otf";
    kohinur.fontName = "Kohinur";
    kohinur.gsubFile = "../FontsProcessed/kohinur/kohinur_gsub.json";
    kohinur.gposFile = "../FontsProcessed/kohinur/kohinur_gpos.json";
    kohinur.nameMapFile = "../FontsProcessed/kohinur/kohinur_nameMap.json";
    kohinur.unicodeMapFile = "../FontsProcessed/kohinur/kohinur_unicodeName.json";
    return kohinur;
}

#endif /* Header_h */
