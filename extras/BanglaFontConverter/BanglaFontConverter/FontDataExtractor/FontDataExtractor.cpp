/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#include <fstream>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <json/json.h>
#pragma clang diagnostic pop
#include <sstream>
#include "common.hpp"
#include "FontDataExtractor.hpp"

map < string, vector < OutGlyph > > FontDataExtractor::getAllConjuncts() {
    return allConjuncts2;
}

map < string, BTOffset > FontDataExtractor::getAllMarkBaseOffset() {
    return allMarkBaseOffset2;
}

FontDataExtractor::FontDataExtractor(FontInfo fontInfo) {
    auto allConjuncts = processGSUBJsonFile(fontInfo.gsubFile);
    auto allMarkPos = processGPOSJsonFile(fontInfo.gposFile);

    nameIndexMap = processNameMapJsonFile(fontInfo.nameMapFile);
    unicodeIndexMap = processUnicodeIndexMapJsonFile(fontInfo.unicodeMapFile);

    for (auto it1:allConjuncts) {
        for (auto it2:it1.second) {
            string key = "";

            auto names = split(it1.first, ',');

            for (int i = 0; i < names.size(); i++) {
                key += to_string(nameIndexMap[names[i]]);

                if (i != 0 && i != names.size() - 1) {
                    key += ",";
                }
            }

            auto names2 = split(it2.first, ',');

            for (int i = 0; i < names2.size(); i++) {
                key += "," + to_string(nameIndexMap[names2[i]]);
            }
            
            vector<OutGlyph> outList;

            for (auto p:it2.second) {
                OutGlyph g;
                g.tag = p.second;
                g.index = nameIndexMap[p.first];
                outList.push_back(g);
            }

            if (allConjuncts2.count(key) > 0) {
                cout << "error" << endl;
                exit(0);
            }

            allConjuncts2[key] = outList;
        }
    }

    for (auto it1:allMarkPos) {
        for (auto it2:it1.second) {
            string key = to_string(nameIndexMap[it1.first]) + "," + to_string(nameIndexMap[it2.first]);
            BTOffset offset;
            offset.x = it2.second.first;
            offset.y = it2.second.second;

            if (allMarkBaseOffset2.count(key) > 0) {
                cout << "error" << endl;
                exit(0);
            }

            allMarkBaseOffset2[key] = offset;
        }
    }
}

map < uint16_t, uint16_t > FontDataExtractor::getUnicodeIndexMap() {
    return unicodeIndexMap;
}

map < uint16_t, uint16_t > FontDataExtractor::processUnicodeIndexMapJsonFile(string unicodeMapFile) {
    map < uint16_t, uint16_t > unicodeIndexMap;

    ifstream file(unicodeMapFile);
    Json::Value root;

    file >> root;

    for (auto child : root) {
        unicodeIndexMap[child["code"].asInt()] = nameIndexMap[child["name"].asString()];
    }

    return unicodeIndexMap;
}

map < string, int> FontDataExtractor::processNameMapJsonFile(string filepath) {
    map < string, int> nameMap;

    ifstream file(filepath);
    Json::Value root;

    file >> root;

    for (auto child : root) {
        nameMap[child["name"].asString()] = stoi(child["id"].asString());
    }

    return nameMap;
}

map<string, map<string, pair<int, int> > > FontDataExtractor::processGPOSJsonFile(string filepath) {
    ifstream file(filepath);
    Json::Value root;

    file >> root;

    map<string, map<string, pair<int, int> > > pp;

    for (auto child : root) {
        for (auto mark : child["mark_glyphs"]) {
            map<string, pair<int, int> > bb;

            for (auto base : child["base_glyphs"]) {
                bb[base.asString()] = make_pair(0, 0);
            }

            pp[mark.asString()] = bb;
        }
    }

    return pp;
}

map<string, map<string, vector<pair<string, string> > > > FontDataExtractor::processGSUBJsonFile(string filepath) {
    ifstream file(filepath);
    Json::Value root;

    file >> root;

    map<string, map<string, vector<pair<string, string> > > > allConjuncts;

    for (auto child : root) {
        vector<string> inputs;

        for (auto in : child["input_glyphs"]) {
            inputs.push_back(in.asString());
        }

        if (allConjuncts.count(inputs[0])) {
            allConjuncts[inputs[0]][inputs[1]].push_back({ child["output"].asString(), child["tag"].asString() });
        } else {
            map<string, vector<pair<string, string> > > mm;
            mm[inputs[1]].push_back({ child["output"].asString(), child["tag"].asString() });
            allConjuncts[inputs[0]] = mm;
        }
    }

    return allConjuncts;
}

void FontDataExtractor::logGSUBData() {
    int l = 0;

    for (auto it : allConjuncts2) {
        l++;
        cout <<  it.first << ", " << " output:";

        for (auto v : it.second) {
            cout <<  v.index << ",";
        }

        cout << endl;
    }

    cout << l << endl;
}

void FontDataExtractor::logGPOSData() {
    int l = 0;

    for (auto it : allMarkBaseOffset2) {
        l++;
        cout <<  it.first << " (x:" << it.second.x << " y:" << it.second.y << ")" << endl;
    }

    cout << l << endl;
}
