/*
 * Author: Mamunul Mazid
 * License: The MIT License (MIT)
 * Copyright (c) 2024 Mamunul Mazid
 */

#include "BTGlyphDecoder.hpp"

BTGlyphDecoder::BTGlyphDecoder(BTAbstractUtility *helper) : helper(helper)
{
}

int16_t BTGlyphDecoder::findConjuctGlyphIndexFrom3Components(std::vector<int16_t> indexArray, std::string featureTag)
{
    int16_t index = -1;

    std::string component1st = std::to_string(indexArray[0]);
    std::string component2nd = std::to_string(indexArray[1]);
    std::string component3rd = std::to_string(indexArray[2]);

    std::string key = component1st + "," + component2nd + "," + component3rd;
    index = helper->findConjunct(key, featureTag);
    return index;
}

int16_t BTGlyphDecoder::findConjuctGlyphIndexFrom2Components(std::vector<int16_t> indexArray, std::string featureTag)
{
    int16_t index = -1;

    std::string component1st = std::to_string(indexArray[0]);
    std::string component2nd = std::to_string(indexArray[1]);

    std::string key = component1st + "," + component2nd;
    index = helper->findConjunct(key, featureTag);

    return index;
}

std::vector<int16_t> BTGlyphDecoder::getGlyphIndexArrayFromText(std::string text)
{
    std::vector<int16_t> unicodeArray = helper->decodeStringToUnicode(text);
    std::vector<int16_t> indexArray = convertUnicodeToIndex(unicodeArray);
    indexArray = matchFota(indexArray);
    indexArray = matchJaFola(indexArray);
    indexArray = matchRef(indexArray);
    indexArray = matchBaFolaGlyph(indexArray);
    indexArray = matchRaFolaGlyph(indexArray);
    indexArray = matchConjuctAKhandGlyph(indexArray);
    indexArray = matchTrippleConsonantGlyph(indexArray);
    indexArray = matchTwoConsonantWithRaFolaGlyph(indexArray);
    indexArray = matchTwoConsonantWithBaFolaGlyph(indexArray);
    indexArray = matchTwoConsonantGlyph(indexArray);
    indexArray = mergeConjuctAndBaFola(indexArray);
    indexArray = mergeConjuctAndRaFola(indexArray);
    indexArray = rearrangeKar(indexArray);
    indexArray = rearrangeTwoVowel(indexArray);
    indexArray = matchUVowelGlyph(indexArray);
    indexArray = matchUUVowelGlyph(indexArray);
    indexArray = matchRiVowelGlyph(indexArray);
    indexArray = matchIIVowelGlyph(indexArray);
    indexArray = rearrangeRef(indexArray);
    indexArray = rearrangeUKar(indexArray);

    return indexArray;
}

int16_t BTGlyphDecoder::getIndexOfJaFola()
{
    std::vector<int16_t> glyphArray = {helper->getGlyphIndexOfUnicode(Virama), helper->getGlyphIndexOfUnicode(Ja)};
    std::string featureTag = "pstf";
    int16_t index = findConjuctGlyphIndexFrom2Components(glyphArray, featureTag);

    return index; // 150;//u09CD_u09AF
}

int16_t BTGlyphDecoder::getIndexOfRaFola()
{
    std::vector<int16_t> glyphArray = {helper->getGlyphIndexOfUnicode(Virama), helper->getGlyphIndexOfUnicode(RaMiddle)};
    std::string featureTag = "blwf";
    int16_t index = findConjuctGlyphIndexFrom2Components(glyphArray, featureTag);

    return index; // 436; //u09F0_u09CD
}

int16_t BTGlyphDecoder::getIndexOfBaFola()
{
    std::vector<int16_t> glyphArray = {helper->getGlyphIndexOfUnicode(Virama), helper->getGlyphIndexOfUnicode(Ba)};
    std::string featureTag = "blwf";
    int16_t index = findConjuctGlyphIndexFrom2Components(glyphArray, featureTag);

    return index; // 467; //u09AC_u09CD
}

int16_t BTGlyphDecoder::getIndexOfRef()
{
    std::vector<int16_t> glyphArray = {helper->getGlyphIndexOfUnicode(Ra), helper->getGlyphIndexOfUnicode(Virama)};
    std::string featureTag = "rphf";
    int16_t index = findConjuctGlyphIndexFrom2Components(glyphArray, featureTag);

    return index; // 435;//u09B0_u09CD
}

std::vector<int16_t> BTGlyphDecoder::matchRaFolaGlyph(std::vector<int16_t> indexArray)
{
    int8_t i = 1;

    while (i < (int8_t)indexArray.size())
    {
        if (indexArray[i - 1] == helper->getGlyphIndexOfUnicode(Virama) &&
            indexArray[i] == helper->getGlyphIndexOfUnicode(Ra))
        {
            int index = getIndexOfRaFola(); 
            indexArray[i - 1] = index;
            indexArray.erase(indexArray.begin() + i);
        }
        else
        {
            i++;
        }
    }
    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::matchBaFolaGlyph(std::vector<int16_t> indexArray)
{
    int8_t i = 0;

    while (i < (int8_t)indexArray.size())
    {
        if (indexArray[i] == helper->getGlyphIndexOfUnicode(Ba) &&
            indexArray[i - 1] == helper->getGlyphIndexOfUnicode(Virama))
        {
            int16_t index = getIndexOfBaFola();
            indexArray[i - 1] = index;
            indexArray.erase(indexArray.begin() + i);
        }
        else
        {
            i++;
        }
    }
    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::mergeConjuctAndBaFola(std::vector<int16_t> indexArray)
{
    int8_t i = 1;

    while (i < (int8_t)indexArray.size())
    {
        if (indexArray[i] == getIndexOfBaFola())
        {
            std::vector<int16_t> glyphArray = {indexArray[i - 1], indexArray[i]};
            std::string featureTag = "blws";
            int16_t index1 = findConjuctGlyphIndexFrom2Components(glyphArray, featureTag);

            if (index1 == -1)
                return indexArray;

            indexArray[i - 1] = index1;
            indexArray.erase(indexArray.begin() + i);
        }

        i++;
    }

    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::mergeConjuctAndRaFola(std::vector<int16_t> indexArray)
{
    int8_t i = 1;

    while (i < (int8_t)indexArray.size())
    {
        if (indexArray[i] == getIndexOfRaFola())
        {
            std::vector<int16_t> glyphArray = {indexArray[i - 1], indexArray[i]};
            std::string featureTag = "blws";
            int16_t index1 = findConjuctGlyphIndexFrom2Components(glyphArray, featureTag);
            if (index1 == -1)
                return indexArray;
            indexArray[i - 1] = index1;
            indexArray.erase(indexArray.begin() + i);
        }

        i++;
    }

    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::matchConjuctAKhandGlyph(std::vector<int16_t> indexArray)
{
    std::string featureTag = "akhn";
    int8_t i = 0;
    while (i < (int8_t)(indexArray.size() - 2))
    {
        if (indexArray[i] == helper->getGlyphIndexOfUnicode(Ka) &&
            indexArray[i + 1] == helper->getGlyphIndexOfUnicode(Virama) &&
            indexArray[i + 2] == helper->getGlyphIndexOfUnicode(MurdhannoSha))
        {
            std::vector<int16_t> glyphArray3 = {indexArray[i], indexArray[i + 1], indexArray[i + 2]};
            int16_t index2 = findConjuctGlyphIndexFrom3Components(glyphArray3, featureTag);
            indexArray[i] = index2;
            indexArray.erase(indexArray.begin() + i + 1);
            indexArray.erase(indexArray.begin() + i + 1);
        }

        if (indexArray[i] == helper->getGlyphIndexOfUnicode(BorgioJa) &&
            indexArray[i + 1] == helper->getGlyphIndexOfUnicode(Virama) &&
            indexArray[i + 2] == helper->getGlyphIndexOfUnicode(Nya))
        {
            std::vector<int16_t> glyphArray3 = {indexArray[i], indexArray[i + 1], indexArray[i + 2]};
            int16_t index2 = findConjuctGlyphIndexFrom3Components(glyphArray3, featureTag);
            indexArray[i] = index2;
            indexArray.erase(indexArray.begin() + i + 1);
            indexArray.erase(indexArray.begin() + i + 1);
        }

        i++;
    }
    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::matchTwoConsonantWithBaFolaGlyph(std::vector<int16_t> indexArray)
{
    int8_t i = 1;

    while (i < (int8_t)indexArray.size())
    {
        if (indexArray[i] == helper->getGlyphIndexOfUnicode(Virama) && indexArray[i + 2] == getIndexOfBaFola())
        {
            std::vector<int16_t> glyphArray = {indexArray[i - 1], indexArray[i]};
            std::string featureTag = "half";
            int16_t index1 = findConjuctGlyphIndexFrom2Components(glyphArray, featureTag);
            std::vector<int16_t> glyphArray2 = {indexArray[i + 1], indexArray[i + 2]};
            std::string featureTag2 = "vatu";
            int16_t index2 = findConjuctGlyphIndexFrom2Components(glyphArray2, featureTag2);

            std::vector<int16_t> glyphArray3 = {index1, index2};
            std::string featureTag3 = "pres";
            int16_t index3 = findConjuctGlyphIndexFrom2Components(glyphArray3, featureTag3);

            if (index3 <= 0)
            {
                i++;
                continue;
            }

            indexArray[i - 1] = index3;
            indexArray.erase(indexArray.begin() + i);
            indexArray.erase(indexArray.begin() + i);
            indexArray.erase(indexArray.begin() + i);
        }
        else
        {
            i++;
        }
    }

    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::matchTwoConsonantWithRaFolaGlyph(std::vector<int16_t> indexArray)
{
    int8_t i = 1;

    while (i < (int8_t)indexArray.size())
    {
        if (indexArray[i] == helper->getGlyphIndexOfUnicode(Virama) && indexArray[i + 2] == getIndexOfRaFola())
        {
            std::vector<int16_t> glyphArray = {indexArray[i - 1], indexArray[i]};
            std::string featureTag = "half";
            int16_t index1 = findConjuctGlyphIndexFrom2Components(glyphArray, featureTag);
            std::vector<int16_t> glyphArray2 = {indexArray[i + 1], indexArray[i + 2]};
            std::string featureTag2 = "vatu";
            int16_t index2 = findConjuctGlyphIndexFrom2Components(glyphArray2, featureTag2);

            std::vector<int16_t> glyphArray3 = {index1, index2};
            std::string featureTag3 = "pres";
            int16_t index3 = findConjuctGlyphIndexFrom2Components(glyphArray3, featureTag3);

            if (index3 <= 0)
            {
                i++;
                continue;
            }

            indexArray[i - 1] = index3;
            indexArray.erase(indexArray.begin() + i);
            indexArray.erase(indexArray.begin() + i);
            indexArray.erase(indexArray.begin() + i);
        }
        else
        {
            i++;
        }
    }

    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::matchTwoConsonantGlyph(std::vector<int16_t> indexArray)
{
    // u0995_u09CD_u09B7

    int8_t i = 1;

    while (i < (int8_t)indexArray.size())
    {
        if (indexArray[i] == helper->getGlyphIndexOfUnicode(Virama))
        {
            std::vector<int16_t> glyphArray = {indexArray[i - 1], helper->getGlyphIndexOfUnicode(Virama)};
            std::string featureTag = "half";
            int16_t index1 = findConjuctGlyphIndexFrom2Components(glyphArray, featureTag);

            std::vector<int16_t> glyphArray2 = {index1, indexArray[i + 1]};
            featureTag = "pres";
            int16_t index2 = findConjuctGlyphIndexFrom2Components(glyphArray2, featureTag);

            if (index2 <= 0)
            {
                std::vector<int16_t> glyphArray3 = {indexArray[i - 1], indexArray[i], indexArray[i + 1]};
                index2 = findConjuctGlyphIndexFrom3Components(glyphArray3, featureTag);

                if (index2 <= 0)
                {
                    i++;
                    continue;
                }
            }

            indexArray[i - 1] = index2;
            indexArray.erase(indexArray.begin() + i);
            indexArray.erase(indexArray.begin() + i);
        }
        else
        {
            i++;
        }
    }
    return indexArray;
}

// akhn

std::vector<int16_t> BTGlyphDecoder::matchTrippleConsonantGlyph(std::vector<int16_t> indexArray)
{
    int8_t i = 1;
    while (i < (int8_t)(indexArray.size() - 2))
    {
        if (indexArray[i] == helper->getGlyphIndexOfUnicode(Virama) &&
            indexArray[i + 2] == helper->getGlyphIndexOfUnicode(Virama))
        {
            std::vector<int16_t> glyphArray = {indexArray[i - 1], indexArray[i]};
            std::string featureTag = "half";
            int16_t index1 = findConjuctGlyphIndexFrom2Components(glyphArray, featureTag);
            std::vector<int16_t> glyphArray2 = {indexArray[i + 1], indexArray[i + 2]};
            int16_t index2 = findConjuctGlyphIndexFrom2Components(glyphArray2, featureTag);
            std::vector<int16_t> glyphArray3 = {index1, index2, indexArray[i + 3]};
            int16_t index = findConjuctGlyphIndexFrom3Components(glyphArray3, featureTag);

            if (index <= 0)
            {
                i++;
                continue;
            }

            indexArray[i - 1] = index;
            indexArray.erase(indexArray.begin() + i);
            indexArray.erase(indexArray.begin() + i);
            indexArray.erase(indexArray.begin() + i);
            indexArray.erase(indexArray.begin() + i);
        }

        i++;
    }
    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::matchUVowelGlyph(std::vector<int16_t> indexArray)
{
    int8_t i = 1;

    while (i < (int8_t)indexArray.size())
    {
        if (indexArray[i] == helper->getGlyphIndexOfUnicode(UVowel) &&
            indexArray[i - 1] != helper->getGlyphIndexOfUnicode(Virama))
        {
            std::vector<int16_t> glyphArray = {indexArray[i - 1], indexArray[i]};
            std::string featureTag = "blws";
            int16_t index = findConjuctGlyphIndexFrom2Components(glyphArray, featureTag);

            if (index <= 0)
            {
                i++;
                continue;
            }

            indexArray[i - 1] = index;
            indexArray.erase(indexArray.begin() + i);
        }
        else
        {
            i++;
        }
    }
    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::matchUUVowelGlyph(std::vector<int16_t> indexArray)
{
    int8_t i = 1;

    while (i < (int8_t)indexArray.size())
    {
        if (indexArray[i] == helper->getGlyphIndexOfUnicode(UuVowel) &&
            indexArray[i - 1] != helper->getGlyphIndexOfUnicode(Virama))
        {
            std::vector<int16_t> glyphArray = {indexArray[i - 1], indexArray[i]};
            std::string featureTag = "blws";
            int16_t index = findConjuctGlyphIndexFrom2Components(glyphArray, featureTag);

            if (index <= 0)
            {
                i++;
                continue;
            }

            indexArray[i - 1] = index;
            indexArray.erase(indexArray.begin() + i);
        }
        else
        {
            i++;
        }
    }
    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::matchRiVowelGlyph(std::vector<int16_t> indexArray)
{
    int8_t i = 1;

    while (i < (int8_t)indexArray.size())
    {
        if (indexArray[i] == helper->getGlyphIndexOfUnicode(RVowel) &&
            indexArray[i - 1] != helper->getGlyphIndexOfUnicode(Virama))
        {
            std::vector<int16_t> glyphArray = {indexArray[i - 1], indexArray[i]};
            std::string featureTag = "blws";
            int16_t index = findConjuctGlyphIndexFrom2Components(glyphArray, featureTag);

            if (index <= 0)
            {
                i++;
                continue;
            }

            indexArray[i - 1] = index;
            indexArray.erase(indexArray.begin() + i);
        }
        else
        {
            i++;
        }
    }
    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::matchIIVowelGlyph(std::vector<int16_t> indexArray)
{
    int8_t i = 1;

    while (i < (int8_t)indexArray.size())
    {
        if (indexArray[i] == helper->getGlyphIndexOfUnicode(IiVowel) &&
            indexArray[i - 1] != helper->getGlyphIndexOfUnicode(Virama))
        {
            std::vector<int16_t> glyphArray = {indexArray[i - 1], indexArray[i]};
            std::string featureTag = "psts";
            int16_t index = findConjuctGlyphIndexFrom2Components(glyphArray, featureTag);

            if (index <= 0)
            {
                i++;
                continue;
            }

            indexArray[i - 1] = index;
            indexArray.erase(indexArray.begin() + i);
        }
        else
        {
            i++;
        }
    }
    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::matchJaFola(std::vector<int16_t> indexArray)
{
    int8_t i = 1;

    while (i < (int8_t)indexArray.size())
    {
        if (indexArray[i - 1] == helper->getGlyphIndexOfUnicode(Virama) &&
            indexArray[i] == helper->getGlyphIndexOfUnicode(Ja))
        {
            indexArray[i - 1] = getIndexOfJaFola();
            indexArray.erase(indexArray.begin() + i);
        }
        else
        {
            i++;
        }
    }
    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::rearrangeTwoVowel(std::vector<int16_t> indexArray)
{
    int8_t i = 0;

    while (i < (int8_t)indexArray.size())
    {
        if (indexArray[i] == helper->getGlyphIndexOfUnicode(OKar))
        {
            indexArray[i] = helper->getGlyphIndexOfUnicode(AKar);

            if (indexArray[i - 1] == getIndexOfJaFola())
            {
                indexArray.insert(indexArray.begin() + i - 2, helper->getGlyphIndexOfUnicode(EKar));
            }
            else if (i - 1 >= 0)
            {
                indexArray.insert(indexArray.begin() + i - 1, helper->getGlyphIndexOfUnicode(EKar));
            }
            else
            {
                indexArray.insert(indexArray.begin(), helper->getGlyphIndexOfUnicode(EKar));
            }

            i++;
        }
        else if (indexArray[i] == helper->getGlyphIndexOfUnicode(OuKar))
        {
            indexArray[i] = helper->getGlyphIndexOfUnicode(AuKar);

            if (indexArray[i - 1] == getIndexOfJaFola())
            {
                indexArray.insert(indexArray.begin() + i - 2, helper->getGlyphIndexOfUnicode(EKar));
            }
            else if (i - 1 >= 0)
            {
                indexArray.insert(indexArray.begin() + i - 1, helper->getGlyphIndexOfUnicode(EKar));
            }
            else
            {
                indexArray.insert(indexArray.begin(), helper->getGlyphIndexOfUnicode(EKar));
            }

            i++;
        }

        i++;
    }
    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::matchRef(std::vector<int16_t> indexArray)
{
    // 9b0 9cd
    int8_t i = 1;

    while (i < (int8_t)indexArray.size())
    {
        if (indexArray[i - 1] == helper->getGlyphIndexOfUnicode(Ra) &&
            indexArray[i] == helper->getGlyphIndexOfUnicode(Virama))
        {
            int index = getIndexOfRef();
            indexArray[i - 1] = index;
            indexArray.erase(indexArray.begin() + i);
        }
        else
        {
            i++;
        }
    }
    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::rearrangeKar(std::vector<int16_t> indexArray)
{
    // 9c7, 9c8, 9bf

    for (int8_t i = 0; i < (int8_t)indexArray.size(); i++)
    {
        if (indexArray[i] == helper->getGlyphIndexOfUnicode(EKar) ||
            indexArray[i] == helper->getGlyphIndexOfUnicode(OiKar) ||
            indexArray[i] == helper->getGlyphIndexOfUnicode(IKar))
        {
            if (i > 0 && indexArray[i - 1] == getIndexOfJaFola())
            {
                std::swap(indexArray[i], indexArray[i - 2]);
                std::swap(indexArray[i], indexArray[i - 1]);
            }
            else
            {
                std::swap(indexArray[i], indexArray[i - 1]);
            }
        }
    }

    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::rearrangeRef(std::vector<int16_t> indexArray)
{
    for (int8_t i = 0; i < (int8_t)indexArray.size(); i++)
    {
        if (indexArray[i] == getIndexOfRef())
        {
            std::swap(indexArray[i], indexArray[i + 1]);
            i++;

            if (indexArray[i - 1] == helper->getGlyphIndexOfUnicode(EKar) ||
                indexArray[i - 1] == helper->getGlyphIndexOfUnicode(IKar))
            {
                std::swap(indexArray[i], indexArray[i + 1]);
                i++;
            }
        }
    }

    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::rearrangeUKar(std::vector<int16_t> indexArray)
{
    for (int8_t i = 0; i < (int8_t)indexArray.size(); i++)
    {
        if ((indexArray[i] == helper->getGlyphIndexOfUnicode(UVowel) ||
             indexArray[i] == helper->getGlyphIndexOfUnicode(UuVowel) ||
             indexArray[i] == helper->getGlyphIndexOfUnicode(RVowel)) &&
            indexArray[i - 1] == getIndexOfJaFola())
        {
            std::swap(indexArray[i], indexArray[i - 1]);
        }
    }

    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::matchFota(std::vector<int16_t> indexArray)
{
    int8_t i = 1;

    while (i < (int8_t)indexArray.size())
    {
        if (indexArray[i] == helper->getGlyphIndexOfUnicode(Nukta))
        {
            //
            std::vector<int16_t> components = {indexArray[i - 1], indexArray[i]};
            std::string featureTag = "nukt";
            int16_t glyphindex = findConjuctGlyphIndexFrom2Components(components, featureTag);

            indexArray[i - 1] = glyphindex;
            indexArray.erase(indexArray.begin() + i);
        }
        else
        {
            i++;
        }
    }
    return indexArray;
}

std::vector<int16_t> BTGlyphDecoder::convertUnicodeToIndex(std::vector<int16_t> unicodeArray)
{
    std::vector<int16_t> indexArray;

    for (auto code : unicodeArray)
    {
        int index = helper->getGlyphIndexOfUnicode(code);
        indexArray.push_back(index);
    }

    return indexArray;
}
