#!/usr/bin/env python
#
#   Author: Mamunul Mazid
#   License: The MIT License (MIT)
#   Copyright (c) 2024 Mamunul Mazid
#  

from dataclasses import dataclass
import xml.etree.ElementTree as ET 
import json
from fontTools import ttLib

class TTFToXMLExporter:

    @dataclass
    class IndexName:
        id: int
        name: str

    @dataclass
    class UnicodeName:
        code: int
        name: str

    @dataclass
    class GPOS_Ligature:
        base_glyphs = []
        mark_glyphs = []

    @dataclass
    class GSUB_Ligature:
        output: str = ""
        input_glyphs = []
        index: int = 0
        tag: str = ""
        type: int = 0

    def __init__(self, fontFolder):
        self.outputFolderPath = fontFolder.outputFolderPath
        self.outputFileName = fontFolder.outputFileName
        self.fontFile = fontFolder.fontFile

    def parseUnicodeName(self, xmlfile):
        tree = ET.parse(xmlfile)
        root = tree.getroot()
        unicodeMap = []
        cmap2 = root.find("cmap").iter("map")
        for attr in cmap2:
            code = attr.get("code")
            name = attr.get("name")
            glyph = self.UnicodeName(int(code, 0),name)
            unicodeMap.append(glyph)

        return unicodeMap

    def parseNameMap(self, xmlfile):
        tree = ET.parse(xmlfile)

        # get root element
        root = tree.getroot()

        glyph_table = []

        glyphOrder = root.find("GlyphOrder")
        for glyph in glyphOrder.iter("GlyphID"):
            id = glyph.get("id")
            name = glyph.get("name")
            gly = self.IndexName(id, name)
            glyph_table.append(gly)

        return glyph_table

    def parseGPOS(self, xmlfile):
        tree = ET.parse(xmlfile)

        # get root element
        root = tree.getroot()

        gpos_table = []

        gpos = root.find("GPOS")
        lookuplist = gpos.find("LookupList")
        for lookup in lookuplist.iter("Lookup"):
            lig = self.GPOS_Ligature()
            if lookup.find("MarkBasePos") == None:
                continue
            marks = lookup.find("MarkBasePos").find("MarkCoverage")
            markarray = []
            basearray = []
            for child in marks:
                markarray.append(child.get("value"))

            bases = lookup.find("MarkBasePos").find("BaseCoverage")
            for base in bases:
                basearray.append(base.get("value"))

            lig.base_glyphs = basearray
            lig.mark_glyphs = markarray
            gpos_table.append(lig)

        return gpos_table

    def parseGSUB(self, xmlfile):
        tree = ET.parse(xmlfile)

        # get root element
        root = tree.getroot()

        gpos = root.find("GSUB")

        featurelist = gpos.find("FeatureList")

        featureArray = dict()

        for record in featurelist.iter("FeatureRecord"):
            feature_tag = record.find("FeatureTag").get("value")
            for list_index in record.find("Feature").findall("LookupListIndex"):
                featureArray[list_index.get("value")] = feature_tag

        gsub_table = []

        featureTag = [
            "abvs",
            "akhn",
            "blwf",
            "blws",
            "cjct",
            "half",
            "nukt",
            "pstf",
            "rphf",
            "vatu",
            "pres",
            "haln",
            "psts",
            "calt",
        ]
        lookuplist = gpos.find("LookupList")
        for lookup in lookuplist.iter("Lookup"):
            index = lookup.get("index")
            if featureArray.get(index) == None:
                continue
            lookup_type = lookup.find("LookupType").get("value")

            if lookup_type == "1":
                for sub in lookup.find("SingleSubst").findall("Substitution"):
                    if featureArray[index] not in featureTag:
                        continue
                    inputs = []
                    lig = self.GSUB_Ligature()
                    lig.type = lookup_type
                    lig.index = index
                    lig.tag = featureArray[index]
                    inputs.append(sub.get("in"))
                    lig.output = sub.get("out")
                    lig.input_glyphs = inputs
                    gsub_table.append(lig)

            if lookup_type == "4":
                for ligset in lookup.find("LigatureSubst").findall("LigatureSet"):
                    for ligxml in ligset.findall("Ligature"):
                        if featureArray[index] not in featureTag:
                            continue
                        lig = self.GSUB_Ligature()
                        inputs = []
                        lig.type = lookup_type
                        lig.index = index
                        lig.tag = featureArray[index]
                        inputs.append(ligset.get("glyph"))
                        inputs.append(ligxml.get("components"))
                        lig.output = ligxml.get("glyph")
                        lig.input_glyphs = inputs
                        gsub_table.append(lig)

            # if lookup_type == '6':

        return gsub_table

    def generateGPOSJsonFile(self):
        xmlfile = self.outputFolderPath + self.outputFileName + ".xml"
        result = self.parseGPOS(xmlfile)
        json_string = json.dumps(result, default=lambda o: o.__dict__, indent=4)
        # print(json_string)
        file = open(self.outputFolderPath + self.outputFileName + "_gpos.json", "w")
        file.write(json_string)
        file.close()

    def generateGSUBJsonFile(self):
        xmlfile = self.outputFolderPath + self.outputFileName + ".xml"
        # parseGpos(xmlfile)
        result = self.parseGSUB(xmlfile)
        json_string = json.dumps(result, default=lambda o: o.__dict__, indent=4)
        # print(json_string)
        file = open(self.outputFolderPath + self.outputFileName + "_gsub.json", "w")
        file.write(json_string)
        file.close()

    def generateNameMapJsonFile(self):
        xmlfile = self.outputFolderPath + self.outputFileName + ".xml"
        result = self.parseNameMap(xmlfile)
        json_string = json.dumps(result, default=lambda o: o.__dict__, indent=4)
        # print(json_string)
        file = open(self.outputFolderPath + self.outputFileName + "_nameMap.json", "w")
        file.write(json_string)
        file.close()

    def generateUnicodeNameMapJsonFile(self):
        xmlfile = self.outputFolderPath + self.outputFileName + ".xml"
        result = self.parseUnicodeName(xmlfile)
        json_string = json.dumps(result, default=lambda o: o.__dict__, indent=4)
        # print(json_string)
        file = open(self.outputFolderPath + self.outputFileName + "_unicodeName.json", "w")
        file.write(json_string)
        file.close()

    def generateRequiredJsonFiles(self):
        self.exportToXML()
        self.generateGPOSJsonFile()
        self.generateGSUBJsonFile()
        self.generateNameMapJsonFile()
        self.generateUnicodeNameMapJsonFile()

    def exportToXML(self):
        font = ttLib.TTFont(self.fontFile)
        font.saveXML(self.outputFolderPath + self.outputFileName + ".xml")


@dataclass
class FontFolder:
    outputFolderPath = str
    outputFileName = str
    fontFile = str

kalpurush = FontFolder()
kalpurush.outputFolderPath = "../FontsProcessed/kalpurush/"
kalpurush.outputFileName = "kalpurush"
kalpurush.fontFile = "../Fonts/kalpurush.ttf"

bangalMN = FontFolder()
bangalMN.outputFolderPath = "../FontsProcessed/bmn/"
bangalMN.outputFileName = "bmn"
bangalMN.fontFile = "../Fonts/BanglaMN.ttf"

sangamMN = FontFolder()
sangamMN.outputFolderPath = "../FontsProcessed/sangam/"
sangamMN.outputFileName = "bsangammn"
sangamMN.fontFile = "../Fonts/BanglaSangamMN.ttf"

kohinur = FontFolder()
kohinur.outputFolderPath = "../FontsProcessed/kohinur/"
kohinur.outputFileName = "kohinur"
kohinur.fontFile = "../Fonts/KohinoorBanglaRegular.otf"

font = FontFolder()

# font = kalpurush
# font = bangalMN
# font = sangamMN
font = kohinur

parser = TTFToXMLExporter(fontFolder=font)
parser.generateRequiredJsonFiles()
